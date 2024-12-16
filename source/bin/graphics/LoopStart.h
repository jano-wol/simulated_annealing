#ifndef SIMULATED_ANNEALING_GRAPHICS_LOOP_START_H_
#define SIMULATED_ANNEALING_GRAPHICS_LOOP_START_H_

#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

void LoopStart()
{
  glfwPollEvents();
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}

#endif  // SIMULATED_ANNEALING_GRAPHICS_LOOP_START_H_
