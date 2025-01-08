#ifndef SIMULATED_ANNEALING_GRAPHICS_SA_CALL_UI_H_
#define SIMULATED_ANNEALING_GRAPHICS_SA_CALL_UI_H_

#include <future>

#include <bs/BS_thread_pool.hpp>

#include <core/IPosition.h>
#include <sa/SAFactory.h>

class SACallUI
{
public:
  void saCallUpdate(bool isSimulating);
  void startSimulating(const sa::core::IPosition::CPtr& currPosition, const sa::sa::SAFactory::CPtr& saFactory);
  void postProcessResults(const sa::core::IPosition::CPtr& allTimeBest, bool trackBest,
                          const std::string& allTimeBestFile, std::vector<sa::sa::SA::CPtr> results);

  bool saCalled = false;
  BS::multi_future<sa::sa::SA::CPtr> simulatingFutures;
  std::future<std::pair<sa::sa::SA::CPtr, sa::core::IPosition::CPtr>> resultFuture;
  std::atomic<double> progress = std::atomic<double>(0);
  std::atomic<bool> stop = std::atomic<bool>(false);
};

#endif  // SIMULATED_ANNEALING_GRAPHICS_SA_CALL_UI_H_
