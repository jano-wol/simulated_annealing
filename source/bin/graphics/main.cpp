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
      ImGui::InputDouble("##DurationInSecondsInput", &state.durationInSeconds, 0.0, 0.0, "%.2f");
    }
    if (state.currentResourceIndex == 1) {
      ImGui::Text("Iteration:");
      ImGui::SameLine();
      ImGui::InputScalar("##IterationInput", ImGuiDataType_U64, &state.iteration);
    }

    ImGui::Text("Acceptance Policy:");
    ImGui::SameLine();
    ImGui::PushItemWidth(ImGui::GetWindowHeight() * 0.15f);
    ImGui::Combo("##AcceptancePolicy", &state.currentAcceptanceIndex, state.acceptanceNames.data(),
                 state.acceptanceNames.size());
    if (state.currentAcceptanceIndex == 0) {
      ImGui::SameLine();
      ImGui::Text("Normalizator:");
      ImGui::SameLine();
      ImGui::InputDouble("##NormalizatorInput", &state.normalizator, 0.0, 0.0, "%.2f");
    }

    ImGui::Text("Cooling Policy:");
    ImGui::SameLine();
    ImGui::PushItemWidth(ImGui::GetWindowHeight() * 0.15f);
    ImGui::Combo("##CoolingPolicy", &state.currentCoolingIndex, state.coolingNames.data(), state.coolingNames.size());
    ImGui::SameLine();
    ImGui::Text("T0:");
    ImGui::SameLine();
    ImGui::InputDouble("##T0Input", &state.t0, 0.0, 0.0, "%.2f");

    ImGui::Text("Move Selector Policy:");
    ImGui::SameLine();
    ImGui::PushItemWidth(ImGui::GetWindowHeight() * 0.15f);
    ImGui::Combo("##MoveSelectorPolicy", &state.currentMoveSelectorIndex, state.moveSelectorNames.data(),
                 state.moveSelectorNames.size());
    ImGui::SameLine();
    ImGui::Text("K:");
    ImGui::SameLine();
    ImGui::InputScalar("##KInput", ImGuiDataType_U32, &state.k);

    ImGui::Text("Monitor:");
    ImGui::SameLine();
    ImGui::PushItemWidth(ImGui::GetWindowHeight() * 0.15f);
    ImGui::Combo("##MonitorLevel", &state.currentMonitorIndex, state.monitorNames.data(), state.monitorNames.size());
    if (state.currentMonitorIndex == 0) {
      ImGui::SameLine();
      ImGui::Text("BestCatchQ:");
      ImGui::SameLine();
      ImGui::InputDouble("##BestQ0Input", &state.bestCatchQ, 0.0, 0.0, "%.2f");
      if (state.bestCatchQ < 0.0) {
        state.bestCatchQ = 0.0;
      }
      if (state.bestCatchQ > 1.0) {
        state.bestCatchQ = 1.0;
      }
    }
    if (state.currentMonitorIndex == 1) {
      ImGui::SameLine();
      ImGui::Text("Snapshots:");
      ImGui::SameLine();
      ImGui::InputScalar("##Snapshots1Input", ImGuiDataType_U32, &state.steps);
      ImGui::SameLine();
      ImGui::Text("LocalEnvLength:");
      ImGui::SameLine();
      ImGui::InputScalar("##localEnvLength1Input", ImGuiDataType_U32, &state.localEnvLength);
      ImGui::SameLine();
      ImGui::Text("BestCatchQ:");
      ImGui::SameLine();
      ImGui::InputDouble("##BestQ1Input", &state.bestCatchQ, 0.0, 0.0, "%.2f");
      if (state.bestCatchQ < 0.0) {
        state.bestCatchQ = 0.0;
      }
      if (state.bestCatchQ > 1.0) {
        state.bestCatchQ = 1.0;
      }
      ImGui::SameLine();
      ImGui::Text("MemoryLimit(Gb):");
      ImGui::SameLine();
      ImGui::InputScalar("##MemoryLimit1Input", ImGuiDataType_U32, &state.memoryLimitInGb);
      ImGui::SameLine();
    }
    if (state.currentMonitorIndex == 2) {
      ImGui::SameLine();
      ImGui::Text("LocalEnvLength:");
      ImGui::SameLine();
      ImGui::InputScalar("##localEnvLength2Input", ImGuiDataType_U32, &state.localEnvLength);
      ImGui::SameLine();
      ImGui::Text("BestCatchQ:");
      ImGui::SameLine();
      ImGui::InputDouble("##BestQ2Input", &state.bestCatchQ, 0.0, 0.0, "%.2f");
      if (state.bestCatchQ < 0.0) {
        state.bestCatchQ = 0.0;
      }
      if (state.bestCatchQ > 1.0) {
        state.bestCatchQ = 1.0;
      }
      ImGui::SameLine();
      ImGui::Text("MemoryLimit(Gb):");
      ImGui::SameLine();
      ImGui::InputScalar("##MemoryLimit2Input", ImGuiDataType_U32, &state.memoryLimitInGb);
      ImGui::SameLine();
    }    

    ImGui::PopItemWidth();

    windowEnd();
    render(window);
  }
  GLFWStop(window);
  return 0;
}
