#ifndef SIMULATED_ANNEALING_GRAPHICS_SA_FACTORY_H_
#define SIMULATED_ANNEALING_GRAPHICS_SA_FACTORY_H_

#include "UIState.h"

void saFactoryUpdate(UIState& state)
{
  ImGui::Text("Resource Policy:");
  ImGui::SameLine();
  ImGui::PushItemWidth(ImGui::GetWindowHeight() * 0.15f);
  ImGui::Combo("##ResourcePolicy", &state.loadingSAFactoryParams.resourceIndex, state.resourceNames.data(),
               state.resourceNames.size());
  ImGui::SameLine();
  if (state.loadingSAFactoryParams.resourceIndex == 0) {
    ImGui::Text("Duration(s):");
    ImGui::SameLine();
    ImGui::InputDouble("##DurationInSecondsInput", &state.loadingSAFactoryParams.durationInSeconds, 0.0, 0.0, "%.2f");
  }
  if (state.loadingSAFactoryParams.resourceIndex == 1) {
    ImGui::Text("Iteration:");
    ImGui::SameLine();
    ImGui::InputScalar("##IterationInput", ImGuiDataType_U64, &state.loadingSAFactoryParams.iteration);
  }

  ImGui::Text("Acceptance Policy:");
  ImGui::SameLine();
  ImGui::PushItemWidth(ImGui::GetWindowHeight() * 0.15f);
  ImGui::Combo("##AcceptancePolicy", &state.loadingSAFactoryParams.acceptanceIndex, state.acceptanceNames.data(),
               state.acceptanceNames.size());
  if (state.loadingSAFactoryParams.acceptanceIndex == 0) {
    ImGui::SameLine();
    ImGui::Text("Normalizator:");
    ImGui::SameLine();
    ImGui::InputDouble("##NormalizatorInput", &state.loadingSAFactoryParams.normalizator, 0.0, 0.0, "%.2f");
  }

  ImGui::Text("Cooling Policy:");
  ImGui::SameLine();
  ImGui::PushItemWidth(ImGui::GetWindowHeight() * 0.15f);
  ImGui::Combo("##CoolingPolicy", &state.loadingSAFactoryParams.coolingIndex, state.coolingNames.data(),
               state.coolingNames.size());
  if (state.loadingSAFactoryParams.coolingIndex == 3 || state.loadingSAFactoryParams.coolingIndex == 4) {
    ImGui::SameLine();
    ImGui::Text("C:");
    ImGui::SameLine();
    ImGui::InputDouble("##CInput", &state.loadingSAFactoryParams.c, 0.0, 0.0, "%.2f");
  }
  ImGui::SameLine();
  ImGui::Text("T0:");
  ImGui::SameLine();
  ImGui::InputDouble("##T0Input", &state.loadingSAFactoryParams.t0, 0.0, 0.0, "%.2f");

  ImGui::Text("Move Selector Policy:");
  ImGui::SameLine();
  ImGui::PushItemWidth(ImGui::GetWindowHeight() * 0.15f);
  ImGui::Combo("##MoveSelectorPolicy", &state.loadingSAFactoryParams.moveSelectorIndex, state.moveSelectorNames.data(),
               state.moveSelectorNames.size());
  ImGui::SameLine();
  ImGui::Text("K:");
  ImGui::SameLine();
  ImGui::InputScalar("##KInput", ImGuiDataType_U32, &state.loadingSAFactoryParams.k);

  ImGui::Text("Monitor:");
  ImGui::SameLine();
  ImGui::PushItemWidth(ImGui::GetWindowHeight() * 0.15f);
  ImGui::Combo("##MonitorLevel", &state.loadingSAFactoryParams.monitorIndex, state.monitorNames.data(),
               state.monitorNames.size());
  if (state.loadingSAFactoryParams.monitorIndex == 0) {
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
  if (state.loadingSAFactoryParams.monitorIndex == 1) {
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
  }
  if (state.loadingSAFactoryParams.monitorIndex == 2) {
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
  }
  ImGui::PopItemWidth();
  state.updateSAFactory();
}

#endif  // SIMULATED_ANNEALING_GRAPHICS_SA_FACTORY_H_
