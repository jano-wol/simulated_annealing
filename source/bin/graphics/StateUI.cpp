#include "StateUI.h"

#include <io/Io.h>

using namespace sa::core;
using namespace sa::io;
using namespace sa::sa;

void StateUI::updateParsing()
{
  if (isParsing) {
    if (menuUI.parsingFuture.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
      loadingPosition = menuUI.parsingFuture.get();
      if (loadingPosition) {
        currentPosition = std::move(loadingPosition);
      }
      isParsing = false;
    }
  }
}

void StateUI::updateSaving()
{
  if (isSaving) {
    if (menuUI.savingFuture.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
      isSaving = false;
    }
  }
}

void StateUI::updateSAFactory()
{
  if (!saFactory || (loadingSAFactoryParams != currentSAFactoryParams)) {
    saFactory = loadingSAFactoryParams.getFactory();
    currentSAFactoryParams = loadingSAFactoryParams;
  }
}

void StateUI::handleMenu()
{
  menuUI.menuUpdate();

  if (menuUI.loadedPath != menuUI.currPath) {
    if (mtx.try_lock()) {
      if (menuUI.mode == 1) {
        isParsing = true;
        menuUI.startParsing();
        menuUI.currPath = menuUI.loadedPath;
      }
      if (menuUI.mode == 2) {
        isSaving = true;
        menuUI.startSaving(currentPosition);
        menuUI.currPath = menuUI.loadedPath;
      }
      mtx.unlock();
    } else {
      menuUI.loadedPath = menuUI.currPath;
      // std::cout << "Warning: decrementCounter() could not acquire the lock! Mutex is busy." << std::endl;
    }
  }
  updateParsing();
  updateSaving();
}