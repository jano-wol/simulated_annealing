#ifndef SIMULATED_ANNEALING_GRAPHICS_SA_FACTORY_H_
#define SIMULATED_ANNEALING_GRAPHICS_SA_FACTORY_H_

#include <imgui/imgui.h>

#include "UIState.h"

void readDoubleNonNeg(const char* text, const char* id, double* val)
{
  ImGui::TextUnformatted(text);
  ImGui::SameLine();
  ImGui::InputDouble(id, val, 0.0, 0.0, "%.2f");
  if (*val < 0) {
    *val = 0;
  }
}

void readDouble01(const char* text, const char* id, double* val)
{
  readDoubleNonNeg(text, id, val);
  if (*val > 1) {
    *val = 1;
  }
}

void readU32(const char* text, const char* id, int* val)
{
  ImGui::TextUnformatted(text);
  ImGui::SameLine();
  ImGui::InputScalar(id, ImGuiDataType_U32, val);
  if (*val < 0) {
    *val = 0;
  }
}

void readU64(const char* text, const char* id, std::size_t* val)
{
  ImGui::TextUnformatted(text);
  ImGui::SameLine();
  ImGui::InputScalar(id, ImGuiDataType_U64, val);
}

void combo(const char* text, const char* id, int* index, const std::vector<const char*>& v)
{
  ImGui::TextUnformatted(text);
  ImGui::SameLine();
  ImGui::Combo(id, index, v.data(), v.size());
}

void saFactoryUpdate(UIState& state)
{
  ImGui::PushItemWidth(ImGui::GetWindowHeight() * 0.15f);

  combo("Resource Policy:", "##ResourcePolicy", &state.loadingSAFactoryParams.resourceIndex, state.resourceNames);
  ImGui::SameLine();
  if (state.loadingSAFactoryParams.resourceIndex == 0) {
    readDoubleNonNeg("Duration(s):", "##DurationInSecondsInput", &state.loadingSAFactoryParams.durationInSeconds);
  }
  if (state.loadingSAFactoryParams.resourceIndex == 1) {
    readU64("Iteration:", "##IterationInput", &state.loadingSAFactoryParams.iteration);
  }

  combo("Acceptance Policy:", "##AcceptancePolicy", &state.loadingSAFactoryParams.acceptanceIndex,
        state.acceptanceNames);
  if (state.loadingSAFactoryParams.acceptanceIndex == 0) {
    ImGui::SameLine();
    readDoubleNonNeg("Normalizator:", "##NormalizatorInput", &state.loadingSAFactoryParams.normalizator);
  }

  combo("Cooling Policy:", "##CoolingPolicy", &state.loadingSAFactoryParams.coolingIndex, state.coolingNames);
  if (state.loadingSAFactoryParams.coolingIndex == 3 || state.loadingSAFactoryParams.coolingIndex == 4) {
    ImGui::SameLine();
    readDoubleNonNeg("C:", "##CInput", &state.loadingSAFactoryParams.c);
  }
  ImGui::SameLine();
  readDoubleNonNeg("T0:", "##T0Input", &state.loadingSAFactoryParams.t0);

  combo("Move Selector Policy:", "##MoveSelectorPolicy", &state.loadingSAFactoryParams.moveSelectorIndex,
        state.moveSelectorNames);
  ImGui::SameLine();
  readU32("K:", "##KInput", &state.loadingSAFactoryParams.k);

  combo("Monitor:", "##MonitorLevel", &state.loadingSAFactoryParams.monitorIndex, state.monitorNames);
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
