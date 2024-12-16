#include "GLFWInit.h"
#include "GLFWStop.h"
#include "LoopStart.h"
#include "Menubar.h"
#include "Render.h"
#include "Scale.h"
#include "UIState.h"

using namespace sa::io;

int main()
{
  GLFWwindow* window = GLFWInit("SA");
  scale();
  UIState state;
  while (!glfwWindowShouldClose(window)) {
    LoopStart();
    {
      ImGuiViewport* viewport = ImGui::GetMainViewport();
      ImGui::SetNextWindowPos(viewport->Pos);
      ImGui::SetNextWindowSize(viewport->Size);
      ImGuiWindowFlags window_flags = 0;
      window_flags |= ImGuiWindowFlags_MenuBar;
      window_flags |= ImGuiWindowFlags_NoTitleBar;
      window_flags |= ImGuiWindowFlags_NoResize;
      bool cloasble = true;
      ImGui::Begin("graphics", &cloasble, window_flags);
      auto path = imgui_menubar::menuBarFile();
      if (!path.empty() && state.readyToCompute()) {
        state.startParsing(path);
      }
      state.updateParsing();
      ImGui::End();
    }
    render(window);
  }
  GLFWStop(window);
  return 0;
}
