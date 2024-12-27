#include "StateUI.h"

#include <sstream>

#include <implot/implot.h>

#include <core/Rounding.h>
#include <io/Io.h>

using namespace sa::core;
using namespace sa::io;
using namespace sa::sa;

void StateUI::updateParsing()
{
  if (isParsing) {
    if (menuUI.parsingFuture.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
      auto loadingPosition = menuUI.parsingFuture.get();
      if (loadingPosition) {
        currentPosition = std::move(loadingPosition);
      } else {
        updateInformating("Parsing failed.");
      }
      ImPlot::SetNextAxesToFit();
      sa = nullptr;
      isParsing = false;
      mtx.unlock();
    }
  }
}

void StateUI::updateSaving()
{
  if (isSaving) {
    if (menuUI.savingFuture.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
      auto success = menuUI.savingFuture.get();
      if (!success) {
        updateInformating("Saving failed.");
      }
      isSaving = false;
      mtx.unlock();
    }
  }
}

void StateUI::updateInformating(const std::string& message)
{
  if (isInformating == false) {
    isInformating = true;
    infoUI.message = message;
  }
}

void StateUI::updateSimulating()
{
  if (isSimulating) {
    if (saCallUI.simulatingFuture.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
      isSimulating = false;
      saCallUI.progress = 0;
      saCallUI.stop.store(false);
      mtx.unlock();
    }
  }
}

void StateUI::handleInfo()
{
  if (isInformating) {
    infoUI.infoUpdate();
  }
  if (infoUI.okPushed) {
    isInformating = false;
  }
}

void StateUI::handleMenu()
{
  menuUI.menuUpdate();

  if (menuUI.operationRequest) {
    if (mtx.try_lock()) {
      if (menuUI.mode == 1) {
        isParsing = true;
        menuUI.startParsing();
      }
      if (menuUI.mode == 2) {
        isSaving = true;
        menuUI.startSaving(currentPosition);
      }
    } else {
      std::stringstream ss;
      ss << "Computation is busy. " << menuUI.title << " request ignored.";
      updateInformating(ss.str());
    }
  }
  updateParsing();
  updateSaving();
}

void StateUI::handleSAFactory()
{
  saFactoryUI.saFactoryUpdate();
  if (!saFactory || (saFactoryUI.loadedParams != saFactoryUI.currentParams)) {
    saFactory = saFactoryUI.loadedParams.getFactory(saCallUI.progress, saCallUI.stop);
    saFactoryUI.currentParams = saFactoryUI.loadedParams;
  }
}

void StateUI::handleSACall()
{
  saCallUI.saCallUpdate(isSimulating);
  if (saCallUI.saCalled) {
    if (currentPosition) {
      if (mtx.try_lock()) {
        sa = saFactory->create();
        isSimulating = true;
        saCallUI.startSimulating(currentPosition, sa, pool);
        saCallUI.saCalled = false;
      } else {
        saCallUI.saCalled = false;
        updateInformating("Computation is busy. Simulation request ignored.");
      }
    } else {
      saCallUI.saCalled = false;
      updateInformating("Start position is not loaded. Simulation request ignored.");
    }
  }
  updateSimulating();
}

void StateUI::handleResults()
{
  if (sa && !isSimulating) {
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
    mtx.unlock();
  }
}

void StateUI::handleGraphics()
{
  static int currentStep = 0;
  const int totalSteps = 20;
  if (currentPosition) {
    ImVec2 availableSize = ImGui::GetContentRegionAvail();
    ImVec2 plotSize = ImVec2(availableSize.x, availableSize.x);
    ImPlot::BeginPlot("", {plotSize.x, plotSize.y}, ImPlotFlags_NoLegend);
    int axisFlag = ImPlotAxisFlags_NoDecorations | ImPlotAxisFlags_NoHighlight;
    ImPlot::SetupAxis(ImAxis_X1, nullptr, axisFlag);
    ImPlot::SetupAxis(ImAxis_Y1, nullptr, axisFlag);
    currentPosition->plot();
    ImPlot::EndPlot();
    ImGui::SetNextItemWidth(plotSize.x);
    if (ImGui::SliderInt("##SnapshotSlider", &currentStep, 0, totalSteps - 1)) {
    }
    const ImGuiStyle& style = ImGui::GetStyle();
    float button_width_1 = ImGui::CalcTextSize("<").x + style.FramePadding.x * 2.0f;
    float button_width_2 = ImGui::CalcTextSize("<<").x + style.FramePadding.x * 2.0f;
    float spacing = ImGui::GetStyle().ItemSpacing.x;
    float total_width = (button_width_1 + button_width_2) * 2 + spacing * 3;
    float center_offset = (plotSize.x - total_width) / 2.0f;
    ImGui::SetCursorPosX(center_offset);
    if (ImGui::Button("<<")) {
      // Move to beginning
    }
    ImGui::SameLine();
    if (ImGui::Button("<")) {
      // Move to previous
    }
    ImGui::SameLine();
    if (ImGui::Button(">")) {
      // Move to next
    }
    ImGui::SameLine();
    if (ImGui::Button(">>")) {
      // Move to end
    }
  }
}

void StateUI::handleSAOutput()
{
  ImVec2 windowSize = ImGui::GetContentRegionAvail();
  float graphicsRatio = 0.7f;
  float graphicsWidth = windowSize.x * graphicsRatio;
  auto style = ImGui::GetStyle();
  if (currentPosition) {
    float navigatorRatio = 0.45f;
    ImGui::BeginChild("Graphics panel", ImVec2(graphicsWidth, 0), 0, ImGuiWindowFlags_NoDecoration);
    ImGui::BeginChild("Navigator", ImVec2(graphicsWidth * navigatorRatio, 0), 0, ImGuiWindowFlags_NoDecoration);
    handleGraphics();
    ImGui::EndChild();
    ImGui::SameLine();
    if (sa && !isSimulating) {
      ImGui::BeginChild("Right Panel", ImVec2(graphicsWidth * (1 - navigatorRatio) - 3 * style.WindowPadding.x, 0), 0,
                        ImGuiWindowFlags_NoDecoration);
      handleResults();
      ImGui::EndChild();
    }
    ImGui::EndChild();
  }
}