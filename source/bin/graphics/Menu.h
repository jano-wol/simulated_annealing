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
        state.fileBrowser.title = "Open";
        state.fileBrowser.mode = 1;
      }
      if (ImGui::MenuItem("Save")) {
        state.fileBrowser.loadedVisible = true;
        state.fileBrowser.title = "Save";
        state.fileBrowser.mode = 2;
      }
      ImGui::EndMenu();
    }
    ImGui::EndMenuBar();
  }

  state.fileBrowser.render();
  if (state.fileBrowser.loadedPath != state.fileBrowser.currPath) {
    if (state.fileBrowser.mode == 1) {
      state.startParsing(state.fileBrowser.loadedPath);
      state.fileBrowser.currPath = state.fileBrowser.loadedPath;
    }
    if (state.fileBrowser.mode == 2) {
      state.startSaving(state.fileBrowser.loadedPath);
      state.fileBrowser.currPath = state.fileBrowser.loadedPath;
    }
  }
  state.updateParsing();
  state.updateSaving();
}

#endif  // SIMULATED_ANNEALING_GRAPHICS_MENU_H_
