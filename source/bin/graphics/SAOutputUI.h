#ifndef SIMULATED_ANNEALING_GRAPHICS_SA_OUTPUT_UI_H_
#define SIMULATED_ANNEALING_GRAPHICS_SA_OUTPUT_UI_H_

#include <imgui/imgui.h>

#include <core/IPosition.h>
#include <sa/SA.h>

class SAOutputUI
{
public:
  void handleButtons(const sa::sa::SA::CPtr& sa, float plotSize);
  void handleNavigator(const sa::sa::SA::CPtr& sa, float plotSize);
  void handlePlot(const sa::core::IPosition::CPtr& plotPosition, float plotSize);
  void handleResults(const sa::sa::SA::CPtr& sa);
  void saOutputUpdate(const sa::core::IPosition::CPtr& plotPosition, const sa::sa::SA::CPtr& sa, bool isSimulating);

  int snapshotIdx = 0;
  const ImVec4 disabledColor = {0.3f, 0.3f, 0.3f, 1.0f};
};

#endif  // SIMULATED_ANNEALING_GRAPHICS_SA_OUTPUT_UI_H_
