#ifndef SIMULATED_ANNEALING_SA_STUDIO_WINDOW_BEGIN_H_
#define SIMULATED_ANNEALING_SA_STUDIO_WINDOW_BEGIN_H_

#include <imgui/imgui.h>

void windowBegin()
{
  static bool cloasble = true;
  ImGuiViewport* viewport = ImGui::GetMainViewport();
  ImGui::SetNextWindowPos(viewport->Pos);
  ImGui::SetNextWindowSize(viewport->Size);
  ImGuiWindowFlags window_flags = 0;
  window_flags |= ImGuiWindowFlags_MenuBar;
  window_flags |= ImGuiWindowFlags_NoTitleBar;
  window_flags |= ImGuiWindowFlags_NoResize;
  ImGui::Begin("graphics", &cloasble, window_flags);
}

#endif  // SIMULATED_ANNEALING_SA_STUDIO_WINDOW_BEGIN_H_
