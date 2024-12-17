#include "GLFWInit.h"
#include "GLFWStop.h"
#include "LoopStart.h"
#include "Menu.h"
#include "Render.h"
#include "Scale.h"
#include "UIState.h"
#include "WindowBegin.h"
#include "WindowEnd.h"

using namespace sa::io;

int main()
{
  GLFWwindow* window = GLFWInit("SA");
  scale();
  UIState state;
  while (!glfwWindowShouldClose(window)) {
    loopStart();
    windowBegin();
    menu(state);
    // Dropdown selection
    ImGui::Text("Resource Policy:");
    ImGui::SameLine();
    ImGui::PushItemWidth(ImGui::GetWindowHeight() * 0.15f);
    ImGui::Combo("##ResourcePolicy", &state.currentResourceIndex, state.resourceNames.data(),
                 state.resourceNames.size());
    ImGui::SameLine();
    if (state.currentResourceIndex == 0) {
      ImGui::Text("Duration(s):");
      ImGui::SameLine();
      if (ImGui::InputDouble("##DurationInSecondsInput", &state.durationInSeconds, 0.0, 0.0, "%.2f")) {
      }
    }
    if (state.currentResourceIndex == 1) {
      ImGui::Text("Iteration:");
      ImGui::SameLine();
      if (ImGui::InputScalar("##IterationInput", ImGuiDataType_U64, &state.iteration)) {
      }
    }

    ImGui::Text("Acceptance Policy:");
    ImGui::SameLine();
    ImGui::PushItemWidth(ImGui::GetWindowHeight() * 0.15f);
    ImGui::Combo("##AcceptancePolicy", &state.currentAcceptanceIndex, state.acceptanceNames.data(),
                 state.acceptanceNames.size());
    ImGui::SameLine();
    if (state.currentAcceptanceIndex == 0) {
      ImGui::Text("Normalizator:");
      ImGui::SameLine();
      if (ImGui::InputDouble("##NormalizatorInput", &state.normalizator, 0.0, 0.0, "%.2f")) {
      }
    }

    ImGui::Text("Cooling Policy:");
    ImGui::SameLine();
    ImGui::PushItemWidth(ImGui::GetWindowHeight() * 0.15f);
    ImGui::Combo("##CoolingPolicy", &state.currentAcceptanceIndex, state.acceptanceNames.data(),
                 state.acceptanceNames.size());
    ImGui::SameLine();
    ImGui::Text("T0:");
    ImGui::SameLine();
    if (ImGui::InputDouble("##T0Input", &state.t0, 0.0, 0.0, "%.2f")) {
    }

    ImGui::PopItemWidth();

    windowEnd();
    render(window);
  }
  GLFWStop(window);
  return 0;
}
