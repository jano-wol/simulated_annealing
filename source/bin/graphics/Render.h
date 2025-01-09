#ifndef SIMULATED_ANNEALING_GRAPHICS_RENDER_H_
#define SIMULATED_ANNEALING_GRAPHICS_RENDER_H_

#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

void render(GLFWwindow* window)
{
  const ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
  ImGui::Render();
  int display_w, display_h;
  glfwGetFramebufferSize(window, &display_w, &display_h);
  glViewport(0, 0, display_w, display_h);
  glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w,
               clear_color.w);
  glClear(GL_COLOR_BUFFER_BIT);
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  glfwSwapBuffers(window);
}

#endif  // SIMULATED_ANNEALING_GRAPHICS_RENDER_H_
