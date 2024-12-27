#include "StateUI.h"

#include <sstream>

#include <implot/implot.h>

using namespace sa::core;
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

void StateUI::handleSAOutput() { saOutputUI.saOutputUpdate(currentPosition, sa, isSimulating); }