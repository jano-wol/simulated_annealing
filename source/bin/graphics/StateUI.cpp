#include "StateUI.h"

#include <sstream>

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
      mtx.unlock();
    } else {
      menuUI.loadedPath = menuUI.currentPath;
      std::stringstream ss;
      ss << "Computation is busy." << menuUI.title << " request ignored.";
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
    saFactory = saFactoryUI.loadedParams.getFactory();
    saFactoryUI.currentParams = saFactoryUI.loadedParams;
  }
}