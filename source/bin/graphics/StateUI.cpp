#include "StateUI.h"

#include <sstream>

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

  if (menuUI.loadedPath != menuUI.currentPath) {
    if (mtx.try_lock()) {
      if (menuUI.mode == 1) {
        isParsing = true;
        menuUI.startParsing();
        menuUI.currentPath = menuUI.loadedPath;
      }
      if (menuUI.mode == 2) {
        isSaving = true;
        menuUI.startSaving(currentPosition);
        menuUI.currentPath = menuUI.loadedPath;
      }
    } else {
      menuUI.loadedPath = menuUI.currentPath;
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
    saFactory = saFactoryUI.loadedParams.getFactory(saCallUI.progress);
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
  if (sa && mtx.try_lock()) {
    const auto& globalMetrics = sa->monitor->globalMetrics;
    std::stringstream ss;
    ss << std::setprecision(Rounding::precision) << std::fixed;
    ss << "iterations = " << globalMetrics.idx;
    ImGui::TextUnformatted(ss.str().c_str());
    ss.str("");
    ss << "speed = " << int(globalMetrics.speed) << " (iteration/s)";
    ImGui::TextUnformatted(ss.str().c_str());
    ss.str("");
    ss << "Acceptance ratio = " << double(globalMetrics.acceptance) / double(globalMetrics.idx);
    ImGui::TextUnformatted(ss.str().c_str());
    mtx.unlock();
  }
}

void StateUI::handleGraphics() {}

void StateUI::handleSA()
{
  handleSACall();
  ImVec2 window_size = ImGui::GetContentRegionAvail();
  float left_width = window_size.x * 0.3f;
  float right_width = window_size.x - left_width;
  ImGui::BeginChild("Left Panel", ImVec2(left_width, 0), true);
  handleResults();
  ImGui::EndChild();
  ImGui::SameLine();
  ImGui::BeginChild("Right Panel", ImVec2(right_width, 0), true);
  handleGraphics();
  ImGui::EndChild();
}