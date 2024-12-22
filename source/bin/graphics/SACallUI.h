#ifndef SIMULATED_ANNEALING_GRAPHICS_SA_CALL_UI_H_
#define SIMULATED_ANNEALING_GRAPHICS_SA_CALL_UI_H_

#include <future>

#include <core/IPosition.h>
#include <sa/SA.h>

class SACallUI
{
public:
  void saCallUpdate();
  void startSimulating(const sa::core::IPosition::CPtr& currPosition, sa::sa::SA::CPtr& sa);

  bool saCalled = false;
  std::future<void> simulatingFuture;
};

#endif  // SIMULATED_ANNEALING_GRAPHICS_SA_CALL_UI_H_
