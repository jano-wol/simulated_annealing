#ifndef SIMULATED_ANNEALING_GRAPHICS_STATE_UI_H_
#define SIMULATED_ANNEALING_GRAPHICS_STATE_UI_H_

#include <atomic>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

#include <core/IPosition.h>
#include <io/Io.h>
#include <sa/SAFactory.h>

#include "MenuUI.h"
#include "SAFactoryParams.h"

class StateUI
{
public:
  StateUI()
      : isParsing(false),
        isSaving(false),
        currentPosition(nullptr),
        loadingPosition(nullptr),
        menuUI("Open", sa::io::Io::getTargetsPath())
  {}

  bool isParsing;
  bool isSaving;
  std::unique_ptr<sa::core::IPosition> currentPosition;
  std::unique_ptr<sa::core::IPosition> loadingPosition;
  MenuUI menuUI;

  void updateParsing();
  void updateSaving();

  SAFactoryParams currentSAFactoryParams;
  SAFactoryParams loadingSAFactoryParams;
  sa::sa::SAFactory::CPtr saFactory = nullptr;
  sa::sa::SA::CPtr sa = nullptr;

  std::vector<const char*> resourceNames{"Time", "Iteration"};
  std::vector<const char*> acceptanceNames{"Metropolis", "Greedy"};
  std::vector<const char*> coolingNames{"Linear", "Quadratic", "Cosine", "Exponential", "Logarithmic"};
  std::vector<const char*> moveSelectorNames{"KBest"};
  std::vector<const char*> monitorNames{"Low", "Medium", "High"};

  std::mutex mtx;

  void handleMenu();
  void updateSAFactory();
};

#endif  // SIMULATED_ANNEALING_GRAPHICS_STATE_UI_H_
