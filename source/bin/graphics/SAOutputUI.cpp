#include "SAOutputUI.h"

#include <iomanip>

#include <implot/implot.h>

#include <core/Rounding.h>
#include <monitor/Monitor.h>

using namespace sa::core;
using namespace sa::monitor;
using namespace sa::sa;

std::string toString(std::optional<double> val)
{
  if (val) {
    std::stringstream ss;
    ss << std::setprecision(2) << std::fixed;
    ss << *val;
    return ss.str();

  } else {
    return "nn";
  }
}

void printGlobalMetrics(const GlobalMetrics& globalMetrics, std::stringstream& ss)
{
  ImGui::TextUnformatted("---- Global metrics ----");
  ss << "best energy = " << globalMetrics.bestEnergy
     << " (at prog. = " << Rounding::roundDouble(globalMetrics.bestProgress) << ")";
  ImGui::TextUnformatted(ss.str().c_str());
  ss.str("");
  ss << "iterations = " << globalMetrics.idx;
  ImGui::TextUnformatted(ss.str().c_str());
  ss.str("");
  ss << "speed = " << int(globalMetrics.speed) << " (iteration/s)";
  ImGui::TextUnformatted(ss.str().c_str());
  ss.str("");
  ss << "all acceptance = " << globalMetrics.acceptance
     << "; ratio = " << double(globalMetrics.acceptance) / double(globalMetrics.idx);
  ImGui::TextUnformatted(ss.str().c_str());
  ss.str("");
  ss << "up energy changes = " << globalMetrics.upEnergyChanges
     << "; ratio = " << double(globalMetrics.upEnergyChanges) / double(globalMetrics.idx);
  ImGui::TextUnformatted(ss.str().c_str());
  ss.str("");
}

void printSnapshotMetrics(const SA::CPtr& sa, int snapshotIdx, const GlobalMetrics& globalMetrics,
                          std::stringstream& ss)
{
  const auto& snapshot = sa->monitor->snapshots[snapshotIdx];
  const auto& snapshotMetrics = snapshot.globalMetrics;
  int iterations = 0;
  int acceptances = 0;
  int upEnergyChanges = 0;
  if (0 < snapshotIdx) {
    const auto& snapshotPrev = sa->monitor->snapshots[snapshotIdx - 1];
    iterations = snapshot.globalMetrics.idx - snapshotPrev.globalMetrics.idx;
    acceptances = snapshot.globalMetrics.acceptance - snapshotPrev.globalMetrics.acceptance;
    upEnergyChanges = snapshot.globalMetrics.upEnergyChanges - snapshotPrev.globalMetrics.upEnergyChanges;
  }
  ImGui::TextUnformatted("---- Snapshot metrics ----");
  ss.str("");
  ss << "prog = " << double(snapshotMetrics.idx) / double(globalMetrics.idx) << "\niterations = " << iterations
     << "\nacceptances = " << acceptances << "\nup energy changes = " << upEnergyChanges;
  ImGui::TextUnformatted(ss.str().c_str());
  ss.str("");
}

void printLocalMetrics(const SA::CPtr& sa, int snapshotIdx, std::stringstream& ss)
{
  const auto& candidate = sa->monitor->snapshots[snapshotIdx].candidate;
  const auto& acceptance = sa->monitor->snapshots[snapshotIdx].acceptance;
  ImGui::TextUnformatted("---- Candidate and Acceptance local metrics ----");
  ss.str("");
  ss << std::setprecision(4) << std::fixed;
  ss << "local derivatives = " << candidate.localDerivative << " and " << acceptance.localDerivative;
  ImGui::TextUnformatted(ss.str().c_str());
  ss << std::setprecision(2) << std::fixed;
  ss.str("");
  ss << "e. win. = [" << candidate.minEnergy << ";" << candidate.maxEnergy << "] and [" << acceptance.minEnergy
     << ";" << acceptance.maxEnergy << "]";
  ImGui::TextUnformatted(ss.str().c_str());
  ss.str("");
  ss << "d (mean;deviation) = (" << toString(candidate.deltaStats.mean) << ";"
     << toString(candidate.deltaStats.deviation) << ") and (" << toString(acceptance.deltaStats.mean) << ";"
     << toString(acceptance.deltaStats.deviation) << ")";
  ImGui::TextUnformatted(ss.str().c_str());
  ss.str("");
}

void SAOutputUI::handleButtons(const SA::CPtr& sa, float plotSize)
{
  const ImGuiStyle& style = ImGui::GetStyle();
  float button_width_1 = ImGui::CalcTextSize("<").x + style.FramePadding.x * 2.0f;
  float button_width_2 = ImGui::CalcTextSize("<<").x + style.FramePadding.x * 2.0f;
  float spacing = ImGui::GetStyle().ItemSpacing.x;
  float total_width = (button_width_1 + button_width_2) * 2 + spacing * 3;
  float center_offset = (plotSize - total_width) / 2.0f;
  ImGui::SetCursorPosX(center_offset);
  if (snapshotIdx == 0) {
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
      snapshotIdx = 0;
    }
    ImGui::SameLine();
    bool activateButton = false;
    if (ImGui::Button("<")) {
      activateButton = true;
    }
    if (ImGui::IsKeyPressed(ImGuiKey_LeftArrow)) {
      activateButton = true;
    }
    if (activateButton) {
      --snapshotIdx;
    }
  }
  ImGui::SameLine();
  if (snapshotIdx == int(sa->monitor->snapshots.size() - 1)) {
    ImGui::PushStyleColor(ImGuiCol_Button, disabledColor);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, disabledColor);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, disabledColor);
    ImGui::Button(">");
    ImGui::SameLine();
    ImGui::Button(">>");
    ImGui::SameLine();
    ImGui::PopStyleColor(3);
  } else {
    bool activateButton = false;
    if (ImGui::Button(">")) {
      activateButton = true;
    }
    if (ImGui::IsKeyPressed(ImGuiKey_RightArrow)) {
      activateButton = true;
    }
    if (activateButton) {
      ++snapshotIdx;
    }
    ImGui::SameLine();
    if (ImGui::Button(">>")) {
      snapshotIdx = sa->monitor->snapshots.size() - 1;
    }
  }
}

void SAOutputUI::handleNavigator(const SA::CPtr& sa, float plotSize)
{
  ImGui::SetNextItemWidth(plotSize);
  ImGui::SliderInt("##SnapshotSlider", &snapshotIdx, 0, sa->monitor->snapshots.size() - 1);
  handleButtons(sa, plotSize);
}

void SAOutputUI::handlePlot(const IPosition::CPtr& plotPosition, float plotSize)
{
  ImPlot::BeginPlot("##PositionPlot", {plotSize, plotSize}, ImPlotFlags_NoLegend);
  int axisFlag = ImPlotAxisFlags_NoDecorations | ImPlotAxisFlags_NoHighlight;
  ImPlot::SetupAxis(ImAxis_X1, nullptr, axisFlag);
  ImPlot::SetupAxis(ImAxis_Y1, nullptr, axisFlag);
  plotPosition->plot();
  ImPlot::EndPlot();
}

void SAOutputUI::handleResults(const SA::CPtr& sa)
{
  std::stringstream ss;
  ss << std::setprecision(2) << std::fixed;
  const auto& globalMetrics = sa->monitor->globalMetrics;
  ImGui::TextUnformatted("");
  printSnapshotMetrics(sa, snapshotIdx, globalMetrics, ss);
  ImGui::TextUnformatted("");
  printLocalMetrics(sa, snapshotIdx, ss);
  ImGui::TextUnformatted("");
  printGlobalMetrics(globalMetrics, ss);
}

void SAOutputUI::saOutputUpdate(const IPosition::CPtr& plotPosition, const SA::CPtr& sa, bool isSimulating)
{
  bool simulated = sa && !isSimulating;
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
    handleNavigator(sa, plotSize);
  }
  ImGui::EndChild();
  ImGui::SameLine();
  auto style = ImGui::GetStyle();
  ImVec2 rightPanelSize(graphicsWidth * (1 - plotRatio) - 3 * style.WindowPadding.x, 0);
  ImGui::BeginChild("Right Panel", rightPanelSize, 0, ImGuiWindowFlags_NoDecoration);
  std::stringstream ss;
  ss << std::setprecision(2) << std::fixed;
  ss << "curr energy = " << plotPosition->getEnergy();
  ImGui::TextUnformatted(ss.str().c_str());
  if (simulated) {
    handleResults(sa);
  }
  ImGui::EndChild();
  ImGui::EndChild();
}
