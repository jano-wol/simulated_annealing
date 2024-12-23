#ifndef SIMULATED_ANNEALING_GRAPHICS_SA_CALL_UI_H_
#define SIMULATED_ANNEALING_GRAPHICS_SA_CALL_UI_H_

#include <future>

#include "BS_thread_pool.hpp"

#include <core/IPosition.h>
#include <sa/SA.h>

class SACallUI
{
public:
  void saCallUpdate(bool isSimulating);
  void startSimulating(const sa::core::IPosition::CPtr& currPosition, sa::sa::SA::CPtr& sa, BS::thread_pool<0>& pool);

  bool saCalled = false;
  std::future<void> simulatingFuture;
  std::atomic<double> progress = std::atomic<double>(0);
};

#endif  // SIMULATED_ANNEALING_GRAPHICS_SA_CALL_UI_H_
