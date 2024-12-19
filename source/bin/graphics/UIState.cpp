#include "UIState.h"

#include <io/Io.h>

using namespace sa::core;
using namespace sa::io;
using namespace sa::sa;

void UIState::updateParsing()
{
  if (isParsing) {
    if (fileBrowser.parsingFuture.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
      loadingPosition = fileBrowser.parsingFuture.get();
      if (loadingPosition) {
        currentPosition = std::move(loadingPosition);
      }
      isParsing = false;
    }
  }
}

void UIState::updateSaving()
{
  if (isSaving) {
    if (fileBrowser.savingFuture.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
      isSaving = false;
    }
  }
}

void UIState::updateSAFactory()
{
  if (!saFactory || (loadingSAFactoryParams != currentSAFactoryParams)) {
    saFactory = loadingSAFactoryParams.getFactory();
    currentSAFactoryParams = loadingSAFactoryParams;
  }
}

void UIState::menu()
{
  fileBrowser.menuUpdate();

  if (fileBrowser.loadedPath != fileBrowser.currPath) {
    if (mtx.try_lock()) {
      if (fileBrowser.mode == 1) {
        isParsing = true;
        fileBrowser.startParsing();
        fileBrowser.currPath = fileBrowser.loadedPath;
      }
      if (fileBrowser.mode == 2) {
        isSaving = true;
        fileBrowser.startSaving(currentPosition);
        fileBrowser.currPath = fileBrowser.loadedPath;
      }
    } else {
      fileBrowser.loadedPath = fileBrowser.currPath;
      // std::cout << "Warning: decrementCounter() could not acquire the lock! Mutex is busy." << std::endl;
    }
  }
  updateParsing();
  updateSaving();
}