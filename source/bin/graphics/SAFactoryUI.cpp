#include "SAFactoryUI.h"

#include <imgui/imgui.h>

bool almostEqual(double a, double b, double tolerance = 1e-6) { return std::abs(a - b) <= tolerance; }

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

SAFactoryUI::Params& SAFactoryUI::Params::operator=(const SAFactoryUI::Params& other)
{
  if (this != &other) {
    resourceIndex = other.resourceIndex;
    durationInSeconds = other.durationInSeconds;
    iteration = other.iteration;

    acceptanceIndex = other.acceptanceIndex;
    normalizator = other.normalizator;

    coolingIndex = other.coolingIndex;
    c = other.c;
    t0 = other.t0;

    moveSelectorIndex = other.moveSelectorIndex;
    k = other.k;

    monitorIndex = other.monitorIndex;
    bestCatchQ = other.bestCatchQ;
    localEnvLength = other.localEnvLength;
    steps = other.steps;
    memoryLimitInGb = other.memoryLimitInGb;
  }
  return *this;
}

bool SAFactoryUI::Params::operator==(const SAFactoryUI::Params& other) const
{
  return resourceIndex == other.resourceIndex && almostEqual(durationInSeconds, other.durationInSeconds) &&
         iteration == other.iteration && acceptanceIndex == other.acceptanceIndex &&
         almostEqual(normalizator, other.normalizator) && coolingIndex == other.coolingIndex &&
         almostEqual(c, other.c) && almostEqual(t0, other.t0) && moveSelectorIndex == other.moveSelectorIndex &&
         k == other.k && monitorIndex == other.monitorIndex && almostEqual(bestCatchQ, other.bestCatchQ) &&
         localEnvLength == other.localEnvLength && steps == other.steps && memoryLimitInGb == other.memoryLimitInGb;
}

bool SAFactoryUI::Params::operator!=(const SAFactoryUI::Params& other) const { return !(*this == other); }

sa::policies::IAcceptance::CPtr SAFactoryUI::Params::getAcceptance()
{
  if (acceptanceIndex == 0) {
    return std::make_unique<sa::policies::Metropolis>(normalizator);
  }
  if (acceptanceIndex == 1) {
    return std::make_unique<sa::policies::Greedy>();
  }
  return nullptr;
}

sa::policies::ICooling::CPtr SAFactoryUI::Params::getCooling()
{
  if (coolingIndex == 0) {
    return std::make_unique<sa::policies::Linear>(t0);
  }
  if (coolingIndex == 1) {
    return std::make_unique<sa::policies::Quadratic>(t0);
  }
  if (coolingIndex == 2) {
    return std::make_unique<sa::policies::Cosine>(t0);
  }
  if (coolingIndex == 3) {
    return std::make_unique<sa::policies::Exponential>(c, t0);
  }
  if (coolingIndex == 4) {
    return std::make_unique<sa::policies::Logarithmic>(c, t0);
  }
  return nullptr;
}

sa::policies::IMoveSelector::CPtr SAFactoryUI::Params::getMoveSelector()
{
  if (moveSelectorIndex == 0) {
    return std::make_unique<sa::policies::KBest>(k);
  }
  return nullptr;
}

sa::policies::IResource::CPtr SAFactoryUI::Params::getResource()
{
  if (resourceIndex == 0) {
    return std::make_unique<sa::policies::Time>(durationInSeconds);
  }
  if (resourceIndex == 1) {
    return std::make_unique<sa::policies::Iteration>(iteration);
  }
  return nullptr;
}

sa::monitor::Monitor::CPtr SAFactoryUI::Params::getMonitor(std::atomic<double>& progress)
{
  auto callback = [&progress](double newProgress) { progress.store(newProgress); };
  if (monitorIndex == 0) {
    return std::make_unique<sa::monitor::Monitor>(sa::monitor::MonitorLevel::Low, bestCatchQ);
  }
  if (monitorIndex == 1) {
    return std::make_unique<sa::monitor::Monitor>(sa::monitor::MonitorLevel::Medium, bestCatchQ, 1e-6, localEnvLength,
                                                  steps, memoryLimitInGb * 1000000000UL, std::move(callback));
  }
  if (monitorIndex == 2) {
    return std::make_unique<sa::monitor::Monitor>(sa::monitor::MonitorLevel::High, bestCatchQ, 1e-6, localEnvLength, 20,
                                                  memoryLimitInGb * 1000000000UL, std::move(callback));
  }
  return nullptr;
}

sa::sa::SAFactory::CPtr SAFactoryUI::Params::getFactory(std::atomic<double>& progress)
{
  return std::make_unique<sa::sa::SAFactory>(getResource(), getAcceptance(), getCooling(), getMoveSelector(),
                                             getMonitor(progress));
}

void SAFactoryUI::saFactoryUpdate()
{
  ImGui::PushItemWidth(ImGui::GetWindowHeight() * 0.15f);

  combo("Resource Policy:", "##ResourcePolicy", &loadedParams.resourceIndex, resourceNames);
  ImGui::SameLine();
  if (loadedParams.resourceIndex == 0) {
    readDoubleNonNeg("Duration(s):", "##DurationInSecondsInput", &loadedParams.durationInSeconds);
  }
  if (loadedParams.resourceIndex == 1) {
    readU64("Iteration:", "##IterationInput", &loadedParams.iteration);
  }

  combo("Acceptance Policy:", "##AcceptancePolicy", &loadedParams.acceptanceIndex, acceptanceNames);
  if (loadedParams.acceptanceIndex == 0) {
    ImGui::SameLine();
    readDoubleNonNeg("Normalizator:", "##NormalizatorInput", &loadedParams.normalizator);
  }

  combo("Cooling Policy:", "##CoolingPolicy", &loadedParams.coolingIndex, coolingNames);
  if (loadedParams.coolingIndex == 3 || loadedParams.coolingIndex == 4) {
    ImGui::SameLine();
    readDoubleNonNeg("C:", "##CInput", &loadedParams.c);
  }
  ImGui::SameLine();
  readDoubleNonNeg("T0:", "##T0Input", &loadedParams.t0);

  combo("Move Selector Policy:", "##MoveSelectorPolicy", &loadedParams.moveSelectorIndex, moveSelectorNames);
  ImGui::SameLine();
  readU32("K:", "##KInput", &loadedParams.k);

  combo("Monitor:", "##MonitorLevel", &loadedParams.monitorIndex, monitorNames);
  if (loadedParams.monitorIndex == 0) {
    ImGui::SameLine();
    readDouble01("BestCatchQ:", "##BestQ0Input", &loadedParams.bestCatchQ);
  }
  if (loadedParams.monitorIndex == 1) {
    ImGui::SameLine();
    readU32("Snapshots:", "##Snapshots1Input", &loadedParams.steps);
    ImGui::SameLine();
    readU32("LocalEnvLength:", "##localEnvLength1Input", &loadedParams.localEnvLength);
    ImGui::SameLine();
    readDouble01("BestCatchQ:", "##BestQ1Input", &loadedParams.bestCatchQ);
    ImGui::SameLine();
    readU32("MemoryLimit(Gb):", "##MemoryLimit1Input", &loadedParams.memoryLimitInGb);
  }
  if (loadedParams.monitorIndex == 2) {
    ImGui::SameLine();
    readU32("LocalEnvLength:", "##localEnvLength2Input", &loadedParams.localEnvLength);
    ImGui::SameLine();
    readDouble01("BestCatchQ:", "##BestQ2Input", &loadedParams.bestCatchQ);
    ImGui::SameLine();
    readU32("MemoryLimit(Gb):", "##MemoryLimit2Input", &loadedParams.memoryLimitInGb);
  }
  ImGui::PopItemWidth();
}
