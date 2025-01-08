#include "StateUI.h"
#include "ThreadPoolManager.h"

#include <sstream>

#include <implot/implot.h>

using namespace sa::core;
using namespace sa::sa;

const IPosition::CPtr& StateUI::getPlotPosition() const
{
  if (saOutputUI.monitor && !isAnnealing()) {
    int snapshotIdx = saOutputUI.getSnapshotIdx();
    if (snapshotIdx == -1) {
      return saOutputUI.monitor->bestPosition;
    } else {
      return saOutputUI.monitor->snapshots[snapshotIdx].position;
    }
  } else {
    return currentPosition;
  }
}

bool StateUI::currentPositionPlotted() const
{
  if (currentPosition && (!saOutputUI.monitor || isAnnealing() || (saOutputUI.scrollIdx == 0))) {
    return true;
  }
  return false;
}

bool StateUI::isAnnealing() const { return isSimulating || isPostProcessing; }

void StateUI::updateParsing()
{
  if (isParsing) {
    if (menuUI.parsingFuture.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
      auto [best, loadingPosition] = menuUI.parsingFuture.get();
      if (loadingPosition) {
        currentPosition = std::move(loadingPosition);
        allTimeBest = std::move(best);
        ImPlot::SetNextAxesToFit();
        saOutputUI.monitor = nullptr;
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
    if (saCallUI.simulatingFutures.ready_count() == saCallUI.simulatingFutures.size()) {
      auto results = saCallUI.simulatingFutures.get();
      saCallUI.postProcessResults(allTimeBest, menuUI.trackBest, menuUI.bestFileName, std::move(results));
      isSimulating = false;
      isPostProcessing = true;
    }
  }
  if (isPostProcessing) {
    if (saCallUI.resultFuture.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
      auto [sa, best] = saCallUI.resultFuture.get();
      if (best) {
        allTimeBest = std::move(best);
      }
      saOutputUI.init(std::move(sa->monitor));
      saCallUI.stop.store(false);
      isPostProcessing = false;
      mtx.unlock();
    }
  }
}

void StateUI::updateAllTimeBestLoading()
{
  if (isLoadingAllTime) {
    if (saOutputUI.loadAllTimeBestFuture.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
      auto loadedPosition = saOutputUI.loadAllTimeBestFuture.get();
      if (loadedPosition) {
        currentPosition = std::move(loadedPosition);
        ImPlot::SetNextAxesToFit();
        saOutputUI.monitor = nullptr;
      } else {
        updateInformating("All time best load failed.");
      }
      isLoadingAllTime = false;
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
        menuUI.trackBest = menuUI.trackBestCandidate;
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

void StateUI::handleSAFactory() { saFactoryUI.saFactoryUpdate(); }

void StateUI::handleSACall()
{
  saCallUI.saCallUpdate(isAnnealing());
  if (saCallUI.saCalled) {
    if (currentPosition) {
      if (currentPositionPlotted() == false) {
        currentPosition = getPlotPosition()->clone();
      }
      if (mtx.try_lock()) {
        isSimulating = true;
        std::size_t allTasks = 40;
        std::size_t numberOfThreads = 8;
        auto& pool = ThreadPoolManager::getPool();
        pool.reset(numberOfThreads);
        saCallUI.progresses.clear();
        for (std::size_t i = 0; i < allTasks; ++i) {
          saCallUI.progresses.emplace_back(0);
        }
        saFactoryUI.setRandomSeed();
        auto factories = saFactoryUI.loadedParams.getFactories(saCallUI.progresses, saCallUI.stop);
        saCallUI.startSimulating(currentPosition, std::move(factories));
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
    saOutputUI.saOutputUpdate(plotPosition, allTimeBest, isAnnealing());
    if (saOutputUI.loadAllTimeBest) {
      if (mtx.try_lock()) {
        isLoadingAllTime = true;
        saOutputUI.startLoadingAllTimeBest(allTimeBest);
        saOutputUI.loadAllTimeBest = false;
      } else {
        std::stringstream ss;
        ss << "Computation is busy. Load all-time best request ignored.";
        updateInformating(ss.str());
      }
    }
  }
  updateAllTimeBestLoading();
}
