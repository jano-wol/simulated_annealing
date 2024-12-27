#ifndef SIMULATED_ANNEALING_GRAPHICS_SA_OUTPUT_UI_H_
#define SIMULATED_ANNEALING_GRAPHICS_SA_OUTPUT_UI_H_

#include <core/IPosition.h>
#include <sa/SA.h>

class SAOutputUI
{
public:
  void handleNavigator(const sa::core::IPosition::CPtr& currentPosition);
  void handleResults(const sa::sa::SA::CPtr& sa);
  void saOutputUpdate(const sa::core::IPosition::CPtr& currentPosition, const sa::sa::SA::CPtr& sa, bool isSimulating);
};

#endif  // SIMULATED_ANNEALING_GRAPHICS_SA_OUTPUT_UI_H_
