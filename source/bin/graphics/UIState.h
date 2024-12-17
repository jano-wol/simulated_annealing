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

  bool readyToCompute() const;
  void startParsing(const std::string& path);
  void updateParsing();
};

#endif  // SIMULATED_ANNEALING_GRAPHICS_UI_STATE_H_
