#include "StateUI.h"

#include <sstream>

#include <implot/implot.h>

using namespace sa::core;
using namespace sa::sa;

const IPosition::CPtr& StateUI::getPlotPosition() const
{
  if (sa && !isSimulating) {
    return sa->monitor->snapshots[saOutputUI.snapshotIdx].position;
  } else {
    return currentPosition;
  }
}

bool StateUI::currentPositionPlotted() const
{
  if (currentPosition && (!sa || (isSimulating) || (saOutputUI.snapshotIdx == 0))) {
    return true;
  }
  return false;
}

void StateUI::updateParsing()
{
  if (isParsing) {
    if (menuUI.parsingFuture.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
      auto loadingPosition = menuUI.parsingFuture.get();
      if (loadingPosition) {
        currentPosition = std::move(loadingPosition);
        ImPlot::SetNextAxesToFit();
        sa = nullptr;
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
      saOutputUI.snapshotIdx = sa->monitor->snapshots.size() - 1;
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
        const auto& positionToSave = getPlotPosition();
        menuUI.startSaving(positionToSave);
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
      if (currentPositionPlotted() == false) {
        currentPosition = getPlotPosition()->clone();
      }
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

void StateUI::handleSAOutput()
{
  if (currentPosition) {
    const auto& plotPosition = getPlotPosition();
    saOutputUI.saOutputUpdate(plotPosition, sa, isSimulating);
  }
}