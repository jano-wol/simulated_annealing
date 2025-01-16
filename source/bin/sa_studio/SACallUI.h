#ifndef SIMULATED_ANNEALING_SA_STUDIO_SA_CALL_UI_H_
#define SIMULATED_ANNEALING_SA_STUDIO_SA_CALL_UI_H_

#include <future>

#include <bs/BS_thread_pool.hpp>

#include <core/IPosition.h>
#include <sa/SAFactory.h>

class SACallUI
{
public:
  void saCallUpdate(bool isAnnealing);
  void startSimulating(const sa::core::IPosition::CPtr& currPosition, std::vector<sa::sa::SAFactory::CPtr> factories);
  void postProcessResults(const sa::core::IPosition::CPtr& allTimeBest, bool trackBest,
                          const std::string& allTimeBestFile, std::vector<sa::sa::SA::CPtr> results);

  bool saCalled = false;
  std::atomic<bool> stop = std::atomic<bool>(false);
  std::deque<std::atomic<double>> progresses;
  BS::multi_future<sa::sa::SA::CPtr> simulatingFutures;
  std::future<std::pair<sa::sa::SA::CPtr, sa::core::IPosition::CPtr>> resultFuture;
};

#endif  // SIMULATED_ANNEALING_SA_STUDIO_SA_CALL_UI_H_
