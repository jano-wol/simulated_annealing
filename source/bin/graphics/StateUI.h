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
#include "SAFactoryUI.h"

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
  SAFactoryUI saFactoryUI;

  void updateParsing();
  void updateSaving();

  sa::sa::SAFactory::CPtr saFactory = nullptr;
  sa::sa::SA::CPtr sa = nullptr;
  std::mutex mtx;

  void handleMenu();
  void handleSAFactory();
};

#endif  // SIMULATED_ANNEALING_GRAPHICS_STATE_UI_H_
