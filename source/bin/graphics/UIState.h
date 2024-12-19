#ifndef SIMULATED_ANNEALING_GRAPHICS_UI_STATE_H_
#define SIMULATED_ANNEALING_GRAPHICS_UI_STATE_H_

#include <atomic>
#include <future>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

#include <core/IPosition.h>
#include <io/Io.h>
#include <sa/SAFactory.h>

#include "Menubar.h"
#include "SAFactoryParams.h"

class UIState
{
public:
  UIState()
      : isParsing(false),
        currentPosition(nullptr),
        loadingPosition(nullptr),
        fileBrowser("Open", sa::io::Io::getTargetsPath())
  {}

  std::atomic<bool> isParsing;
  std::atomic<bool> isSaving;
  std::unique_ptr<sa::core::IPosition> currentPosition;
  std::unique_ptr<sa::core::IPosition> loadingPosition;
  FileBrowser fileBrowser;
  std::future<std::unique_ptr<sa::core::IPosition>> parsingFuture;
  std::future<void> savingFuture;

  SAFactoryParams currentSAFactoryParams;
  SAFactoryParams loadingSAFactoryParams;
  sa::sa::SAFactory::CPtr saFactory = nullptr;
  sa::sa::SA::CPtr sa = nullptr;

  std::vector<const char*> resourceNames{"Time", "Iteration"};
  std::vector<const char*> acceptanceNames{"Metropolis", "Greedy"};
  std::vector<const char*> coolingNames{"Linear", "Quadratic", "Cosine", "Exponential", "Logarithmic"};
  std::vector<const char*> moveSelectorNames{"KBest"};
  std::vector<const char*> monitorNames{"Low", "Medium", "High"};

  bool readyToCompute() const;
  void startParsing(const std::string& path);
  void startSaving(const std::string& path);
  void updateParsing();
  void updateSaving();
  void updateSAFactory();
};

#endif  // SIMULATED_ANNEALING_GRAPHICS_UI_STATE_H_
