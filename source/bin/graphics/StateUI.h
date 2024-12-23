#ifndef SIMULATED_ANNEALING_GRAPHICS_STATE_UI_H_
#define SIMULATED_ANNEALING_GRAPHICS_STATE_UI_H_

#include <atomic>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

#include <core/IPosition.h>
#include <sa/SAFactory.h>

#include "BS_thread_pool.hpp"
#include "InfoUI.h"
#include "MenuUI.h"
#include "SACallUI.h"
#include "SAFactoryUI.h"

class StateUI
{
public:
  StateUI() : pool(1) {}
  void handleInfo();
  void handleMenu();
  void handleSAFactory();
  void handleResults();
  void handleGraphics();
  void handleSACall();
  void handleSA();
  void updateParsing();
  void updateSaving();
  void updateInformating(const std::string& message);
  void updateSimulating();

  BS::thread_pool<0> pool;
  bool isParsing = false;
  bool isSaving = false;
  bool isInformating = false;
  bool isSimulating = false;
  InfoUI infoUI;
  MenuUI menuUI;
  SAFactoryUI saFactoryUI;
  SACallUI saCallUI;
  sa::sa::SA::CPtr sa = nullptr;
  sa::sa::SAFactory::CPtr saFactory = nullptr;
  std::unique_ptr<sa::core::IPosition> currentPosition = nullptr;
  std::mutex mtx;
};

#endif  // SIMULATED_ANNEALING_GRAPHICS_STATE_UI_H_
