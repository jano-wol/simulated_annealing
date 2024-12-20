#ifndef SIMULATED_ANNEALING_GRAPHICS_STATE_UI_H_
#define SIMULATED_ANNEALING_GRAPHICS_STATE_UI_H_

#include <atomic>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

#include <core/IPosition.h>
#include <sa/SAFactory.h>

#include "MenuUI.h"
#include "SAFactoryUI.h"

class StateUI
{
public:
  void handleMenu();
  void handleSAFactory();
  void updateParsing();
  void updateSaving();

  bool isParsing = false;
  bool isSaving = false;
  MenuUI menuUI;
  SAFactoryUI saFactoryUI;
  sa::sa::SA::CPtr sa = nullptr;
  sa::sa::SAFactory::CPtr saFactory = nullptr;
  std::unique_ptr<sa::core::IPosition> currentPosition = nullptr;
  std::mutex mtx;
};

#endif  // SIMULATED_ANNEALING_GRAPHICS_STATE_UI_H_
