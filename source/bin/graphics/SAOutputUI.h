#ifndef SIMULATED_ANNEALING_GRAPHICS_SA_OUTPUT_UI_H_
#define SIMULATED_ANNEALING_GRAPHICS_SA_OUTPUT_UI_H_

#include <future>
#include <vector>

#include <imgui/imgui.h>

#include <core/IPosition.h>
#include <sa/SA.h>

class SAOutputUI
{
public:
  void init();
  void handleButtons(float plotSize);
  void handleNavigator(float plotSize);
  void handlePlot(const sa::core::IPosition::CPtr& plotPosition, float plotSize);
  void handleResults(const sa::core::IPosition::CPtr& allTimeBest);
  void handleAllTimeBestButton(float plotSize, const sa::core::IPosition::CPtr& allTimeBest, bool isSimulating);
  void saOutputUpdate(const sa::core::IPosition::CPtr& plotPosition, const sa::core::IPosition::CPtr& allTimeBest,
                      bool isSimulating);
  int getSnapshotIdx() const;
  void startLoadingAllTimeBest(const sa::core::IPosition::CPtr& allTimeBest);

  int scrollIdx = 0;
  int bestScrollIdx = 0;
  double bestValue = 0;
  bool isSnapshotBest = false;
  bool loadAllTimeBest = false;
  sa::monitor::Monitor::CPtr monitor = nullptr;
  std::future<sa::core::IPosition::CPtr> loadAllTimeBestFuture;
  float sliderValue;
  std::vector<double> progresses;
  const ImVec4 disabledColor = {0.3f, 0.3f, 0.3f, 1.0f};
};

#endif  // SIMULATED_ANNEALING_GRAPHICS_SA_OUTPUT_UI_H_
