#ifndef SIMULATED_ANNEALING_SA_STUDIO_GLFW_INIT_H_
#define SIMULATED_ANNEALING_SA_STUDIO_GLFW_INIT_H_

#include <fstream>

#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <implot/implot.h>

static void glfw_error_callback(int error, const char* description)
{
  fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

GLFWwindow* GLFWInit(std::string windowName)
{
  glfwSetErrorCallback(glfw_error_callback);
  if (!glfwInit())
    exit(1);
  const char* glsl_version = "#version 130";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
  GLFWwindow* window =
      glfwCreateWindow(glfwGetVideoMode(glfwGetPrimaryMonitor())->width,
                       glfwGetVideoMode(glfwGetPrimaryMonitor())->height, windowName.c_str(), NULL, NULL);
  if (window == NULL)
    exit(1);
  glfwMakeContextCurrent(window);
  glfwMaximizeWindow(window);
  glfwSwapInterval(1);
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImPlot::CreateContext();
  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);
  ImGuiIO& io = ImGui::GetIO();
  io.IniFilename = NULL;
  io.LogFilename = NULL;
  ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
  return window;
}

#endif  // SIMULATED_ANNEALING_SA_STUDIO_GLFW_INIT_H_
