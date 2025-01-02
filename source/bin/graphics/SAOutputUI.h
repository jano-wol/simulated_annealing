#ifndef SIMULATED_ANNEALING_GRAPHICS_SA_OUTPUT_UI_H_
#define SIMULATED_ANNEALING_GRAPHICS_SA_OUTPUT_UI_H_

#include <vector>

#include <imgui/imgui.h>

#include <core/IPosition.h>
#include <sa/SA.h>

class SAOutputUI
{
public:
  void handleButtons(float plotSize);
  void handleNavigator(float plotSize);
  void handlePlot(const sa::core::IPosition::CPtr& plotPosition, float plotSize);
  void handleResults(const std::unique_ptr<sa::core::IPosition>& allTimeBest, const sa::sa::SA::CPtr& sa);
  void saOutputUpdate(const sa::core::IPosition::CPtr& plotPosition, const std::unique_ptr<sa::core::IPosition>& allTimeBest, const sa::sa::SA::CPtr& sa, bool isSimulating);
  void init(const sa::sa::SA::CPtr& sa);
  int getSnapshotIdx() const;

  int scrollIdx = 0;
  int bestScrollIdx = 0;
  std::vector<double> progresses;
  double bestValue = 0;
  bool isSnapshotBest = false;
  float sliderValue;
  const ImVec4 disabledColor = {0.3f, 0.3f, 0.3f, 1.0f};
};

#endif  // SIMULATED_ANNEALING_GRAPHICS_SA_OUTPUT_UI_H_
