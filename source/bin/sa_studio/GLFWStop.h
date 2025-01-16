#ifndef SIMULATED_ANNEALING_SA_STUDIO_GLFW_STOP_H_
#define SIMULATED_ANNEALING_SA_STUDIO_GLFW_STOP_H_

#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <implot/implot.h>

void GLFWStop(GLFWwindow* window)
{
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImPlot::DestroyContext();
  ImGui::DestroyContext();
  glfwDestroyWindow(window);
  glfwTerminate();
}

#endif  // SIMULATED_ANNEALING_SA_STUDIO_GLFW_STOP_H_
