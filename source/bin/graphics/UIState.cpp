#include "UIState.h"

#include <io/Io.h>

using namespace sa::core;
using namespace sa::io;
using namespace sa::sa;

bool UIState::readyToCompute() const { return !isParsing; }

void UIState::startParsing(const std::string& path)
{
  isParsing = true;
  parsingFuture = std::async(std::launch::async, [path]() { return Io::getPosition(path); });
}

void UIState::updateParsing()
{
  if (isParsing) {
    if (parsingFuture.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
      loadingPosition = parsingFuture.get();  // Retrieve the parsed position
      if (loadingPosition) {
        currentPosition = std::move(loadingPosition);
      }
      isParsing = false;
    }
  }
}

void UIState::updateSAFactory()
{
  if (!saFactory || (loadingSAFactoryParams != currentSAFactoryParams)) {
    saFactory = loadingSAFactoryParams.getFactory();
    sa = saFactory->create();
    currentSAFactoryParams = loadingSAFactoryParams;
  }
}