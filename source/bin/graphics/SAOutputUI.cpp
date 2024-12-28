#include "SAOutputUI.h"

#include <iomanip>

#include <implot/implot.h>

#include <core/Rounding.h>

using namespace sa::core;
using namespace sa::sa;

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
  plotPosition = sa->monitor->snapshots[snapshotIdx].position.get();
  handleButtons(sa, plotSize);
}

void SAOutputUI::handlePlot(float plotSize)
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
  const auto& globalMetrics = sa->monitor->globalMetrics;
  std::stringstream ss;
  ss << std::setprecision(2) << std::fixed;
  ss << "best energy = " << globalMetrics.bestEnergy
     << " (at prog. = " << Rounding::roundDouble(globalMetrics.bestProgress) << ")";
  ImGui::TextUnformatted(ss.str().c_str());
  ss << std::setprecision(Rounding::precision) << std::fixed;
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

void SAOutputUI::saOutputUpdate(const SA::CPtr& sa, bool isSimulating)
{
  if (plotPosition) {
    bool simulated = sa && !isSimulating;
    ImVec2 windowSize = ImGui::GetContentRegionAvail();
    float graphicsRatio = 0.7f;
    float graphicsWidth = windowSize.x * graphicsRatio;
    float plotRatio = 0.45f;
    ImGui::BeginChild("Graphics panel", ImVec2(graphicsWidth, 0), 0, ImGuiWindowFlags_NoDecoration);
    ImGui::BeginChild("Navigator", ImVec2(graphicsWidth * plotRatio, 0), 0, ImGuiWindowFlags_NoDecoration);
    ImVec2 availableSize = ImGui::GetContentRegionAvail();
    float plotSize = availableSize.x;
    handlePlot(plotSize);
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
      ImGui::TextUnformatted("\nGlobal metrics:");
      handleResults(sa);
    }
    ImGui::EndChild();
    ImGui::EndChild();
  }
}
