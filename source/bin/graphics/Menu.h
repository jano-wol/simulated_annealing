#ifndef SIMULATED_ANNEALING_GRAPHICS_MENU_H_
#define SIMULATED_ANNEALING_GRAPHICS_MENU_H_

#include "Menubar.h"
#include "UIState.h"

void menuUpdate(UIState& state)
{
  state.fileBrowser.loadedVisible = false;
  if (ImGui::BeginMenuBar()) {
    if (ImGui::BeginMenu("File")) {
      if (ImGui::MenuItem("Open")) {
        state.fileBrowser.loadedVisible = true;
      }
      if (ImGui::MenuItem("Save")) {
        state.fileBrowser.loadedVisible = true;
      }
      ImGui::EndMenu();
    }
    ImGui::EndMenuBar();
  }

  state.fileBrowser.render();
  if (state.fileBrowser.loadedPath != state.fileBrowser.currPath) {
    state.startParsing(state.fileBrowser.loadedPath);
    state.fileBrowser.currPath = state.fileBrowser.loadedPath;
  }
  state.updateParsing();
}

#endif  // SIMULATED_ANNEALING_GRAPHICS_MENU_H_
