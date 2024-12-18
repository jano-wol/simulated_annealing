#ifndef SIMULATED_ANNEALING_GRAPHICS_SA_FACTORY_H_
#define SIMULATED_ANNEALING_GRAPHICS_SA_FACTORY_H_

#include <imgui/imgui.h>

#include "UIState.h"

void readDoubleNonNeg(std::string text, std::string id, double* val)
{
  ImGui::TextUnformatted(text.c_str());
  ImGui::SameLine();
  ImGui::InputDouble(id.c_str(), val, 0.0, 0.0, "%.2f");
  if (*val < 0) {
    *val = 0;
  }
}

void readDouble01(std::string text, std::string id, double* val)
{
  readDoubleNonNeg(text, id, val);
  if (*val > 1) {
    *val = 1;
  }
}

void readU32(std::string text, std::string id, int* val)
{
  ImGui::TextUnformatted(text.c_str());
  ImGui::SameLine();
  ImGui::InputScalar(id.c_str(), ImGuiDataType_U32, val);
  if (*val < 0) {
    *val = 0;
  }
}

void readU64(std::string text, std::string id, std::size_t* val)
{
  ImGui::TextUnformatted(text.c_str());
  ImGui::SameLine();
  ImGui::InputScalar(id.c_str(), ImGuiDataType_U64, val);
}

void saFactoryUpdate(UIState& state)
{
  ImGui::PushItemWidth(ImGui::GetWindowHeight() * 0.15f);
  ImGui::Text("Resource Policy:");
  ImGui::SameLine();
  ImGui::Combo("##ResourcePolicy", &state.loadingSAFactoryParams.resourceIndex, state.resourceNames.data(),
               state.resourceNames.size());
  ImGui::SameLine();
  if (state.loadingSAFactoryParams.resourceIndex == 0) {
    readDoubleNonNeg("Duration(s):", "##DurationInSecondsInput", &state.loadingSAFactoryParams.durationInSeconds);
  }
  if (state.loadingSAFactoryParams.resourceIndex == 1) {
    readU64("Iteration:", "##IterationInput", &state.loadingSAFactoryParams.iteration);
  }

  ImGui::Text("Acceptance Policy:");
  ImGui::SameLine();
  ImGui::Combo("##AcceptancePolicy", &state.loadingSAFactoryParams.acceptanceIndex, state.acceptanceNames.data(),
               state.acceptanceNames.size());
  if (state.loadingSAFactoryParams.acceptanceIndex == 0) {
    ImGui::SameLine();
    readDoubleNonNeg("Normalizator:", "##NormalizatorInput", &state.loadingSAFactoryParams.normalizator);
  }

  ImGui::Text("Cooling Policy:");
  ImGui::SameLine();
  ImGui::Combo("##CoolingPolicy", &state.loadingSAFactoryParams.coolingIndex, state.coolingNames.data(),
               state.coolingNames.size());
  if (state.loadingSAFactoryParams.coolingIndex == 3 || state.loadingSAFactoryParams.coolingIndex == 4) {
    ImGui::SameLine();
    readDoubleNonNeg("C:", "##CInput", &state.loadingSAFactoryParams.c);
  }
  ImGui::SameLine();
  readDoubleNonNeg("T0:", "##T0Input", &state.loadingSAFactoryParams.t0);

  ImGui::Text("Move Selector Policy:");
  ImGui::SameLine();
  ImGui::Combo("##MoveSelectorPolicy", &state.loadingSAFactoryParams.moveSelectorIndex, state.moveSelectorNames.data(),
               state.moveSelectorNames.size());
  ImGui::SameLine();
  readU32("K:", "##KInput", &state.loadingSAFactoryParams.k);

  ImGui::Text("Monitor:");
  ImGui::SameLine();
  ImGui::Combo("##MonitorLevel", &state.loadingSAFactoryParams.monitorIndex, state.monitorNames.data(),
               state.monitorNames.size());
  if (state.loadingSAFactoryParams.monitorIndex == 0) {
    ImGui::SameLine();
    readDouble01("BestCatchQ:", "##BestQ0Input", &state.loadingSAFactoryParams.bestCatchQ);
  }
  if (state.loadingSAFactoryParams.monitorIndex == 1) {
    ImGui::SameLine();
    readU32("Snapshots:", "##Snapshots1Input", &state.loadingSAFactoryParams.steps);
    ImGui::SameLine();
    readU32("LocalEnvLength:", "##localEnvLength1Input", &state.loadingSAFactoryParams.localEnvLength);
    ImGui::SameLine();
    readDouble01("BestCatchQ:", "##BestQ1Input", &state.loadingSAFactoryParams.bestCatchQ);
    ImGui::SameLine();
    readU32("MemoryLimit(Gb):", "##MemoryLimit1Input", &state.loadingSAFactoryParams.memoryLimitInGb);
  }
  if (state.loadingSAFactoryParams.monitorIndex == 2) {
    ImGui::SameLine();
    readU32("LocalEnvLength:", "##localEnvLength2Input", &state.loadingSAFactoryParams.localEnvLength);
    ImGui::SameLine();
    readDouble01("BestCatchQ:", "##BestQ2Input", &state.loadingSAFactoryParams.bestCatchQ);
    ImGui::SameLine();
    readU32("MemoryLimit(Gb):", "##MemoryLimit2Input", &state.loadingSAFactoryParams.memoryLimitInGb);
  }
  ImGui::PopItemWidth();
  state.updateSAFactory();
}

#endif  // SIMULATED_ANNEALING_GRAPHICS_SA_FACTORY_H_
