#ifndef SIMULATED_ANNEALING_GRAPHICS_SA_OUTPUT_UI_H_
#define SIMULATED_ANNEALING_GRAPHICS_SA_OUTPUT_UI_H_

#include <core/IPosition.h>
#include <sa/SA.h>

class SAOutputUI
{
public:
  void handleNavigator(float plotSize);
  void handlePlot(float plotSize);
  void handleResults(const sa::sa::SA::CPtr& sa);
  void saOutputUpdate(const sa::sa::SA::CPtr& sa, bool isSimulating);

  sa::core::IPosition* plotPosition = nullptr;
};

#endif  // SIMULATED_ANNEALING_GRAPHICS_SA_OUTPUT_UI_H_
