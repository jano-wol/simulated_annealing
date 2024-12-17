#ifndef SIMULATED_ANNEALING_GRAPHICS_UI_STATE_H_
#define SIMULATED_ANNEALING_GRAPHICS_UI_STATE_H_

#include <atomic>
#include <future>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

#include <core/IPosition.h>

class UIState
{
public:
  UIState() : isParsing(false), currentPosition(nullptr), loadingPosition(nullptr) {}

  std::atomic<bool> isParsing;
  std::unique_ptr<sa::core::IPosition> currentPosition;
  std::unique_ptr<sa::core::IPosition> loadingPosition;
  std::future<std::unique_ptr<sa::core::IPosition>> parsingFuture;

  int currentResourceIndex = 0;
  double durationInSeconds = 5.0;
  std::size_t iteration = 1000000;
  std::vector<const char*> resourceNames{"Time", "Iteration"};

  int currentAcceptanceIndex = 0;
  double normalizator = 5.0;
  std::vector<const char*> acceptanceNames{"Metropolis", "Greedy"};

  int currentCoolingIndex = 0;
  double t0 = 1.0;
  std::vector<const char*> coolingNames{"Linear", "Exponential", "Logarithmic", "Quadratic", "Exponential", "Cosine"};

  int currentMoveSelectorIndex = 0;
  int k = 1;
  std::vector<const char*> moveSelectorNames{"KBest"};

  int currentMonitorIndex = 1;
  double bestCatchQ = 0.9;
  int localEnvLength = 1000;
  int steps = 20;
  int memoryLimitInGb = 2;
  std::vector<const char*> monitorNames{"Low", "Medium", "High"};

  bool readyToCompute() const;
  void startParsing(const std::string& path);
  void updateParsing();
};

#endif  // SIMULATED_ANNEALING_GRAPHICS_UI_STATE_H_
