#ifndef SIMULATED_ANNEALING_GRAPHICS_UI_STATE_H_
#define SIMULATED_ANNEALING_GRAPHICS_UI_STATE_H_

#include <atomic>
#include <future>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

#include <core/IPosition.h>
#include <sa/SAFactory.h>

#include "SAFactoryParams.h"

class UIState
{
public:
  UIState() : isParsing(false), currentPosition(nullptr), loadingPosition(nullptr) {}

  std::atomic<bool> isParsing;
  std::unique_ptr<sa::core::IPosition> currentPosition;
  std::unique_ptr<sa::core::IPosition> loadingPosition;
  std::future<std::unique_ptr<sa::core::IPosition>> parsingFuture;

  SAFactoryParams currentSAFactoryParams;
  SAFactoryParams loadingSAFactoryParams;
  sa::sa::SAFactory::CPtr saFactory = nullptr;

  std::vector<const char*> resourceNames{"Time", "Iteration"};
  std::vector<const char*> acceptanceNames{"Metropolis", "Greedy"};
  std::vector<const char*> coolingNames{"Linear", "Exponential", "Logarithmic", "Quadratic", "Exponential", "Cosine"};
  std::vector<const char*> moveSelectorNames{"KBest"};
  std::vector<const char*> monitorNames{"Low", "Medium", "High"};

  bool readyToCompute() const;
  void startParsing(const std::string& path);
  void updateParsing();
  void updateSAFactory();
};

#endif  // SIMULATED_ANNEALING_GRAPHICS_UI_STATE_H_
