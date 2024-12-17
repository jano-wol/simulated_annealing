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
    ImGui::Combo("##ResourcePolicy", &state.loadingSAFactoryParams.currentResourceIndex, state.resourceNames.data(),
                 state.resourceNames.size());
    ImGui::SameLine();
    if (state.loadingSAFactoryParams.currentResourceIndex == 0) {
      ImGui::Text("Duration(s):");
      ImGui::SameLine();
      ImGui::InputDouble("##DurationInSecondsInput", &state.loadingSAFactoryParams.durationInSeconds, 0.0, 0.0, "%.2f");
    }
    if (state.loadingSAFactoryParams.currentResourceIndex == 1) {
      ImGui::Text("Iteration:");
      ImGui::SameLine();
      ImGui::InputScalar("##IterationInput", ImGuiDataType_U64, &state.loadingSAFactoryParams.iteration);
    }

    ImGui::Text("Acceptance Policy:");
    ImGui::SameLine();
    ImGui::PushItemWidth(ImGui::GetWindowHeight() * 0.15f);
    ImGui::Combo("##AcceptancePolicy", &state.loadingSAFactoryParams.currentAcceptanceIndex,
                 state.acceptanceNames.data(), state.acceptanceNames.size());
    if (state.loadingSAFactoryParams.currentAcceptanceIndex == 0) {
      ImGui::SameLine();
      ImGui::Text("Normalizator:");
      ImGui::SameLine();
      ImGui::InputDouble("##NormalizatorInput", &state.loadingSAFactoryParams.normalizator, 0.0, 0.0, "%.2f");
    }

    ImGui::Text("Cooling Policy:");
    ImGui::SameLine();
    ImGui::PushItemWidth(ImGui::GetWindowHeight() * 0.15f);
    ImGui::Combo("##CoolingPolicy", &state.loadingSAFactoryParams.currentCoolingIndex, state.coolingNames.data(),
                 state.coolingNames.size());
    ImGui::SameLine();
    ImGui::Text("T0:");
    ImGui::SameLine();
    ImGui::InputDouble("##T0Input", &state.loadingSAFactoryParams.t0, 0.0, 0.0, "%.2f");

    ImGui::Text("Move Selector Policy:");
    ImGui::SameLine();
    ImGui::PushItemWidth(ImGui::GetWindowHeight() * 0.15f);
    ImGui::Combo("##MoveSelectorPolicy", &state.loadingSAFactoryParams.currentMoveSelectorIndex,
                 state.moveSelectorNames.data(), state.moveSelectorNames.size());
    ImGui::SameLine();
    ImGui::Text("K:");
    ImGui::SameLine();
    ImGui::InputScalar("##KInput", ImGuiDataType_U32, &state.loadingSAFactoryParams.k);

    ImGui::Text("Monitor:");
    ImGui::SameLine();
    ImGui::PushItemWidth(ImGui::GetWindowHeight() * 0.15f);
    ImGui::Combo("##MonitorLevel", &state.loadingSAFactoryParams.currentMonitorIndex, state.monitorNames.data(),
                 state.monitorNames.size());
    if (state.loadingSAFactoryParams.currentMonitorIndex == 0) {
      ImGui::SameLine();
      ImGui::Text("BestCatchQ:");
      ImGui::SameLine();
      ImGui::InputDouble("##BestQ0Input", &state.loadingSAFactoryParams.bestCatchQ, 0.0, 0.0, "%.2f");
      if (state.loadingSAFactoryParams.bestCatchQ < 0.0) {
        state.loadingSAFactoryParams.bestCatchQ = 0.0;
      }
      if (state.loadingSAFactoryParams.bestCatchQ > 1.0) {
        state.loadingSAFactoryParams.bestCatchQ = 1.0;
      }
    }
    if (state.loadingSAFactoryParams.currentMonitorIndex == 1) {
      ImGui::SameLine();
      ImGui::Text("Snapshots:");
      ImGui::SameLine();
      ImGui::InputScalar("##Snapshots1Input", ImGuiDataType_U32, &state.loadingSAFactoryParams.steps);
      ImGui::SameLine();
      ImGui::Text("LocalEnvLength:");
      ImGui::SameLine();
      ImGui::InputScalar("##localEnvLength1Input", ImGuiDataType_U32, &state.loadingSAFactoryParams.localEnvLength);
      ImGui::SameLine();
      ImGui::Text("BestCatchQ:");
      ImGui::SameLine();
      ImGui::InputDouble("##BestQ1Input", &state.loadingSAFactoryParams.bestCatchQ, 0.0, 0.0, "%.2f");
      if (state.loadingSAFactoryParams.bestCatchQ < 0.0) {
        state.loadingSAFactoryParams.bestCatchQ = 0.0;
      }
      if (state.loadingSAFactoryParams.bestCatchQ > 1.0) {
        state.loadingSAFactoryParams.bestCatchQ = 1.0;
      }
      ImGui::SameLine();
      ImGui::Text("MemoryLimit(Gb):");
      ImGui::SameLine();
      ImGui::InputScalar("##MemoryLimit1Input", ImGuiDataType_U32, &state.loadingSAFactoryParams.memoryLimitInGb);
      ImGui::SameLine();
    }
    if (state.loadingSAFactoryParams.currentMonitorIndex == 2) {
      ImGui::SameLine();
      ImGui::Text("LocalEnvLength:");
      ImGui::SameLine();
      ImGui::InputScalar("##localEnvLength2Input", ImGuiDataType_U32, &state.loadingSAFactoryParams.localEnvLength);
      ImGui::SameLine();
      ImGui::Text("BestCatchQ:");
      ImGui::SameLine();
      ImGui::InputDouble("##BestQ2Input", &state.loadingSAFactoryParams.bestCatchQ, 0.0, 0.0, "%.2f");
      if (state.loadingSAFactoryParams.bestCatchQ < 0.0) {
        state.loadingSAFactoryParams.bestCatchQ = 0.0;
      }
      if (state.loadingSAFactoryParams.bestCatchQ > 1.0) {
        state.loadingSAFactoryParams.bestCatchQ = 1.0;
      }
      ImGui::SameLine();
      ImGui::Text("MemoryLimit(Gb):");
      ImGui::SameLine();
      ImGui::InputScalar("##MemoryLimit2Input", ImGuiDataType_U32, &state.loadingSAFactoryParams.memoryLimitInGb);
      ImGui::SameLine();
    }
    ImGui::PopItemWidth();
    state.updateSAFactory();
    windowEnd();
    render(window);
  }
  GLFWStop(window);
  return 0;
}
