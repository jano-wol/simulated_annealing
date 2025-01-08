#include "SAOutputUI.h"

#include <iomanip>

#include <implot/implot.h>

#include <core/Rounding.h>
#include <monitor/Monitor.h>

using namespace sa::core;
using namespace sa::monitor;
using namespace sa::sa;

std::string toShortString(double val)
{
  std::stringstream ss;
  if (val < 100000 && val > -10000) {
    ss << std::setprecision(2) << std::fixed;
    ss << val;
  } else {
    ss << std::scientific << std::setprecision(2) << val;
  }
  return ss.str();
}

float getTotalButtonsWidth()
{
  const ImGuiStyle& style = ImGui::GetStyle();
  float buttonWidth1 = ImGui::CalcTextSize("<").x + style.FramePadding.x * 2.0f;
  float buttonWidth2 = ImGui::CalcTextSize("<<").x + style.FramePadding.x * 2.0f;
  float bestWidth = ImGui::CalcTextSize("best").x + style.FramePadding.x * 2.0f;
  float spacing = ImGui::GetStyle().ItemSpacing.x;
  float totalButtonsWidth = (buttonWidth1 + buttonWidth2) * 2 + bestWidth + spacing * 4;
  return totalButtonsWidth;
}

float getCenterOffset(float plotSize, float totalButtonsWidth)
{
  float centerOffset = (plotSize - totalButtonsWidth) / 2.0f;
  return centerOffset;
}

std::string toString(std::optional<double> val)
{
  if (val) {
    return toShortString(*val);
  } else {
    return "nn";
  }
}

void printGlobalMetrics(const GlobalMetrics& globalMetrics, const sa::core::IPosition::CPtr& allTimeBest,
                        std::stringstream& ss)
{
  ImGui::TextUnformatted("---- Global metrics ----");
  ss.str("");
  ss << "iterations = " << globalMetrics.idx;
  ImGui::TextUnformatted(ss.str().c_str());
  ss.str("");
  ss << "speed = " << int(globalMetrics.speed) << " (iteration/s)";
  ImGui::TextUnformatted(ss.str().c_str());
  ss.str("");
  ss << "all acceptance = " << globalMetrics.acceptance << std::setprecision(4)
     << "; ratio = " << double(globalMetrics.acceptance) / double(globalMetrics.idx) << std::setprecision(2);
  ImGui::TextUnformatted(ss.str().c_str());
  ss.str("");
  ss << "up energy changes = " << globalMetrics.upEnergyChanges << std::setprecision(4)
     << "; ratio = " << double(globalMetrics.upEnergyChanges) / double(globalMetrics.idx) << std::setprecision(2);
  ImGui::TextUnformatted(ss.str().c_str());
  ss.str("");
  ss << "all-time-best = ";
  if (allTimeBest) {
    ss << std::setprecision(4) << std::fixed;
    ss << allTimeBest->getEnergy();
    ss << std::setprecision(2) << std::fixed;
  } else {
    ss << "nn";
  }
  ImGui::TextUnformatted(ss.str().c_str());
  ss.str("");
}

void printSnapshotMetrics(const Monitor::CPtr& monitor, int snapshotIdx, std::stringstream& ss)
{
  const auto& snapshot = monitor->snapshots[snapshotIdx];
  const auto& snapshotMetrics = snapshot.globalMetrics;
  int iterations = 0;
  int acceptances = 0;
  int upEnergyChanges = 0;
  if (0 < snapshotIdx) {
    const auto& snapshotPrev = monitor->snapshots[snapshotIdx - 1];
    iterations = snapshot.globalMetrics.idx - snapshotPrev.globalMetrics.idx;
    acceptances = snapshot.globalMetrics.acceptance - snapshotPrev.globalMetrics.acceptance;
    upEnergyChanges = snapshot.globalMetrics.upEnergyChanges - snapshotPrev.globalMetrics.upEnergyChanges;
  }
  ImGui::TextUnformatted("---- Snapshot metrics ----");
  ss.str("");
  ss << "prog = " << snapshotMetrics.progress << "\niterations = " << iterations << "\nacceptances = " << acceptances
     << "\nup energy changes = " << upEnergyChanges;
  ImGui::TextUnformatted(ss.str().c_str());
  ss.str("");
}

void printLocalMetrics(const Monitor::CPtr& monitor, int snapshotIdx, std::stringstream& ss)
{
  const auto& candidate = monitor->snapshots[snapshotIdx].candidate;
  const auto& acceptance = monitor->snapshots[snapshotIdx].acceptance;
  ImGui::TextUnformatted("---- Candidate and Acceptance local metrics ----");
  ss.str("");
  ss << std::setprecision(4) << std::fixed;
  ss << "local derivatives = " << candidate.localDerivative << " and " << acceptance.localDerivative;
  ImGui::TextUnformatted(ss.str().c_str());
  ss << std::setprecision(2) << std::fixed;
  ss.str("");
  ss << "e. win. = [" << toShortString(candidate.minEnergy) << ";" << toShortString(candidate.maxEnergy) << "] and ["
     << toShortString(acceptance.minEnergy) << ";" << toShortString(acceptance.maxEnergy) << "]";
  ImGui::TextUnformatted(ss.str().c_str());
  ss.str("");
  ss << "d (mean;dev.) = (" << toString(candidate.deltaStats.mean) << ";" << toString(candidate.deltaStats.deviation)
     << ") and (" << toString(acceptance.deltaStats.mean) << ";" << toString(acceptance.deltaStats.deviation) << ")";
  ImGui::TextUnformatted(ss.str().c_str());
  ss.str("");
}

int SAOutputUI::getSnapshotIdx() const
{
  if (isSnapshotBest) {
    return scrollIdx;
  } else {
    if (scrollIdx < bestScrollIdx) {
      return scrollIdx;
    }
    if (scrollIdx > bestScrollIdx) {
      return scrollIdx - 1;
    }
    return -1;
  }
}

void SAOutputUI::init()
{
  bestValue = monitor->snapshots[0].position->getEnergy();
  bestScrollIdx = 0;
  for (int idx = 1; idx < int(monitor->snapshots.size()); ++idx) {
    auto currValue = monitor->snapshots[idx].position->getEnergy();
    if (currValue < bestValue) {
      bestValue = currValue;
      bestScrollIdx = idx;
    }
  }

  isSnapshotBest = true;
  progresses.clear();
  progresses.reserve(monitor->snapshots.size() + 1);
  if (monitor->bestPosition && monitor->bestPosition->getEnergy() < bestValue) {
    isSnapshotBest = false;
    bestValue = monitor->bestPosition->getEnergy();
    double bestProgress = monitor->globalMetrics.bestProgress;
    int idx = 0;
    while (monitor->snapshots[idx].globalMetrics.progress < bestProgress) {
      progresses.push_back(monitor->snapshots[idx].globalMetrics.progress);
      ++idx;
    }
    progresses.push_back(bestProgress);
    bestScrollIdx = idx;
    while (idx < int(monitor->snapshots.size())) {
      progresses.push_back(monitor->snapshots[idx].globalMetrics.progress);
      ++idx;
    }
  } else {
    int idx = 0;
    while (idx < int(monitor->snapshots.size())) {
      progresses.push_back(monitor->snapshots[idx].globalMetrics.progress);
      ++idx;
    }
  }
  scrollIdx = bestScrollIdx;
  sliderValue = progresses[scrollIdx];
}

void SAOutputUI::handleAllTimeBestButton(float plotSize, const IPosition::CPtr& allTimeBest, bool isSimulating)
{
  if (allTimeBest && !isSimulating) {
    float totalButtonsWidth = getTotalButtonsWidth();
    float centerOffset = getCenterOffset(plotSize, totalButtonsWidth);
    ImGui::SetCursorPosX(centerOffset);
    if (monitor) {
      float widgetHeight = ImGui::GetItemRectSize().y;
      float verticalSkip = 0.1f * widgetHeight;
      ImGui::SetCursorPosY(ImGui::GetCursorPosY() + verticalSkip);
    }
    if (ImGui::Button("Load all-time-best", ImVec2(totalButtonsWidth, 0))) {
      loadAllTimeBest = true;
    }
  }
}

void SAOutputUI::handleButtons(float plotSize)
{
  float totalButtonsWidth = getTotalButtonsWidth();
  float centerOffset = getCenterOffset(plotSize, totalButtonsWidth);
  ImGui::SetCursorPosX(centerOffset);
  if (scrollIdx == 0) {
    ImGui::PushStyleColor(ImGuiCol_Button, disabledColor);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, disabledColor);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, disabledColor);
    ImGui::Button("<<");
    ImGui::SameLine();
    ImGui::Button("<");
    ImGui::SameLine();
    ImGui::PopStyleColor(3);
  } else {
    if (ImGui::Button("<<")) {
      scrollIdx = 0;
      sliderValue = float(progresses[scrollIdx]);
    }
    ImGui::SameLine();
    bool activateButton = false;
    if (ImGui::Button("<")) {
      activateButton = true;
    }
    if (!ImGui::IsAnyItemActive() && ImGui::IsKeyPressed(ImGuiKey_LeftArrow)) {
      activateButton = true;
    }
    if (activateButton) {
      --scrollIdx;
      sliderValue = float(progresses[scrollIdx]);
    }
  }
  ImGui::SameLine();
  if (scrollIdx == bestScrollIdx) {
    ImGui::PushStyleColor(ImGuiCol_Button, disabledColor);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, disabledColor);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, disabledColor);
    ImGui::Button("best");
    ImGui::PopStyleColor(3);
  } else {
    if (ImGui::Button("best")) {
      scrollIdx = bestScrollIdx;
      sliderValue = float(progresses[scrollIdx]);
    }
  }
  ImGui::SameLine();
  if (scrollIdx == int(progresses.size() - 1)) {
    ImGui::PushStyleColor(ImGuiCol_Button, disabledColor);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, disabledColor);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, disabledColor);
    ImGui::Button(">");
    ImGui::SameLine();
    ImGui::Button(">>");
    ImGui::PopStyleColor(3);
  } else {
    bool activateButton = false;
    if (ImGui::Button(">")) {
      activateButton = true;
    }
    if (!ImGui::IsAnyItemActive() && ImGui::IsKeyPressed(ImGuiKey_RightArrow)) {
      activateButton = true;
    }
    if (activateButton) {
      ++scrollIdx;
      sliderValue = float(progresses[scrollIdx]);
    }
    ImGui::SameLine();
    if (ImGui::Button(">>")) {
      scrollIdx = progresses.size() - 1;
      sliderValue = float(progresses[scrollIdx]);
    }
  }
  sliderValue = progresses[scrollIdx];
}

void SAOutputUI::handleNavigator(float plotSize)
{
  ImGui::SetNextItemWidth(plotSize);
  if (ImGui::SliderFloat("##SnapshotSlider", &sliderValue, 0, 1, "%.2f")) {
    auto closestIt = std::min_element(progresses.begin(), progresses.end(), [&](float a, float b) {
      return std::abs(a - sliderValue) < std::abs(b - sliderValue);
    });
    scrollIdx = int(std::distance(progresses.begin(), closestIt));
    sliderValue = progresses[scrollIdx];
  }
  handleButtons(plotSize);
}

void SAOutputUI::handlePlot(const IPosition::CPtr& plotPosition, float plotSize)
{
  int plotFlags = ImPlotFlags_NoLegend | ImPlotFlags_Equal;
  ImPlot::BeginPlot("##PositionPlot", {plotSize, plotSize}, plotFlags);
  int axisFlags = ImPlotAxisFlags_NoDecorations | ImPlotAxisFlags_NoHighlight;
  ImPlot::SetupAxis(ImAxis_X1, nullptr, axisFlags);
  ImPlot::SetupAxis(ImAxis_Y1, nullptr, axisFlags);
  plotPosition->plot();
  ImPlot::EndPlot();
}

void SAOutputUI::handleResults(const sa::core::IPosition::CPtr& allTimeBest)
{
  std::stringstream ss;
  ss << std::setprecision(2) << std::fixed;
  int snapshotIdx = getSnapshotIdx();
  ImGui::TextUnformatted("");
  printSnapshotMetrics(monitor, snapshotIdx, ss);
  ImGui::TextUnformatted("");
  printLocalMetrics(monitor, snapshotIdx, ss);
  ImGui::TextUnformatted("");
  printGlobalMetrics(monitor->globalMetrics, allTimeBest, ss);
}

void SAOutputUI::saOutputUpdate(const IPosition::CPtr& plotPosition, const sa::core::IPosition::CPtr& allTimeBest,
                                bool isSimulating)
{
  bool simulated = monitor && !isSimulating;
  loadAllTimeBest = false;
  ImVec2 windowSize = ImGui::GetContentRegionAvail();
  float graphicsRatio = 0.7f;
  float graphicsWidth = windowSize.x * graphicsRatio;
  float plotRatio = 0.45f;
  ImGui::BeginChild("Graphics panel", ImVec2(graphicsWidth, 0), 0, ImGuiWindowFlags_NoDecoration);
  ImGui::BeginChild("Navigator", ImVec2(graphicsWidth * plotRatio, 0), 0, ImGuiWindowFlags_NoDecoration);
  ImVec2 availableSize = ImGui::GetContentRegionAvail();
  float plotSize = availableSize.x;
  handlePlot(plotPosition, plotSize);
  if (simulated) {
    handleNavigator(plotSize);
  }
  handleAllTimeBestButton(plotSize, allTimeBest, isSimulating);
  ImGui::EndChild();
  ImGui::SameLine();
  auto style = ImGui::GetStyle();
  ImVec2 rightPanelSize(graphicsWidth * (1 - plotRatio) - 3 * style.WindowPadding.x, 0);
  ImGui::BeginChild("Right Panel", rightPanelSize, 0, ImGuiWindowFlags_NoDecoration);
  std::stringstream ss;
  ss << std::setprecision(4) << std::fixed;
  ss << "curr energy = " << plotPosition->getEnergy();
  ImGui::TextUnformatted(ss.str().c_str());
  if (simulated) {
    if ((isSnapshotBest || (scrollIdx != bestScrollIdx))) {
      handleResults(allTimeBest);
    } else {
      ImGui::TextUnformatted("");
      printGlobalMetrics(monitor->globalMetrics, allTimeBest, ss);
    }
  }
  ImGui::EndChild();
  ImGui::EndChild();
}

void SAOutputUI::startLoadingAllTimeBest(const IPosition::CPtr& allTimeBest)
{
  loadAllTimeBestFuture = std::async(std::launch::async, [&allTimeBest]() { return allTimeBest->clone(); });
}
