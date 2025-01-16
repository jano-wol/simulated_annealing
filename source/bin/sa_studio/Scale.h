#ifndef SIMULATED_ANNEALING_SA_STUDIO_SCALE_H_
#define SIMULATED_ANNEALING_SA_STUDIO_SCALE_H_

#include <GLFW/glfw3.h>
#include <imgui/imgui.h>

#include <io/Io.h>

void scale()
{
  int characterInRow = 90;
  int fontSize = (glfwGetVideoMode(glfwGetPrimaryMonitor())->width) / characterInRow;
  std::string fontPath = sa::io::Io::getFontPath();
  ImGuiIO& io = ImGui::GetIO();
  (void)io.Fonts->AddFontFromFileTTF(fontPath.c_str(), float(fontSize));
  ImGui::GetStyle().ScaleAllSizes((float)fontSize / 13.0f);
}

#endif  // SIMULATED_ANNEALING_SA_STUDIO_SCALE_H_
