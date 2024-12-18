#ifndef SIMULATED_ANNEALING_GRAPHICS_MENU_H_
#define SIMULATED_ANNEALING_GRAPHICS_MENU_H_

#include "Menubar.h"
#include "UIState.h"

void menuUpdate(UIState& state)
{
  auto path = imgui_menubar::menuBarFile();
  if (!path.empty() && state.readyToCompute()) {
    state.startParsing(path);
  }
  state.updateParsing();
}

#endif  // SIMULATED_ANNEALING_GRAPHICS_MENU_H_
