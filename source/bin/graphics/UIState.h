#ifndef SIMULATED_ANNEALING_GRAPHICS_UI_STATE_H_
#define SIMULATED_ANNEALING_GRAPHICS_UI_STATE_H_

#include <atomic>
#include <future>
#include <memory>
#include <mutex>
#include <string>

#include <core/IPosition.h>

class UIState
{
public:
  UIState() : isParsing(false), currentPosition(nullptr), loadingPosition(nullptr) {}

  // State variables
  std::atomic<bool> isParsing;
  std::unique_ptr<sa::core::IPosition> currentPosition;
  std::unique_ptr<sa::core::IPosition> loadingPosition;
  std::future<std::unique_ptr<sa::core::IPosition>> parsingFuture;

  bool readyToCompute() const;
  void startParsing(const std::string& path);
  void updateParsing();
};

#endif  // SIMULATED_ANNEALING_GRAPHICS_UI_STATE_H_
