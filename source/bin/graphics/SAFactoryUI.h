#ifndef SIMULATED_ANNEALING_GRAPHICS_SA_FACTORY_UI_H_
#define SIMULATED_ANNEALING_GRAPHICS_SA_FACTORY_UI_H_

#include <imgui/imgui.h>

#include <monitor/Monitor.h>
#include <policies/Acceptance.h>
#include <policies/Cooling.h>
#include <policies/MoveSelector.h>
#include <policies/Resource.h>
#include <sa/SAFactory.h>

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

class SAFactoryUI
{
public:
  class Params
  {
  public:
    Params& operator=(const Params& other)
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

    static constexpr double epsilon = 1e-6;
    static bool almostEqual(double a, double b, double tolerance = epsilon) { return std::abs(a - b) <= tolerance; }

    int resourceIndex = 0;
    double durationInSeconds = 5.0;
    std::size_t iteration = 1000000;

    int acceptanceIndex = 0;
    double normalizator = 1.0;

    int coolingIndex = 0;
    double c = 1.0;
    double t0 = 1.0;

    int moveSelectorIndex = 0;
    int k = 1;

    int monitorIndex = 1;
    double bestCatchQ = 0.9;
    int localEnvLength = 1000;
    int steps = 20;
    int memoryLimitInGb = 2;

    bool operator==(const Params& other) const
    {
      return resourceIndex == other.resourceIndex && almostEqual(durationInSeconds, other.durationInSeconds) &&
             iteration == other.iteration && acceptanceIndex == other.acceptanceIndex &&
             almostEqual(normalizator, other.normalizator) && coolingIndex == other.coolingIndex &&
             almostEqual(c, other.c) && almostEqual(t0, other.t0) && moveSelectorIndex == other.moveSelectorIndex &&
             k == other.k && monitorIndex == other.monitorIndex && almostEqual(bestCatchQ, other.bestCatchQ) &&
             localEnvLength == other.localEnvLength && steps == other.steps && memoryLimitInGb == other.memoryLimitInGb;
    }

    bool operator!=(const Params& other) const { return !(*this == other); }

    sa::policies::IAcceptance::CPtr getAcceptance()
    {
      if (acceptanceIndex == 0) {
        return std::make_unique<sa::policies::Metropolis>(normalizator);
      }
      if (acceptanceIndex == 1) {
        return std::make_unique<sa::policies::Greedy>();
      }
      return nullptr;
    }

    sa::policies::ICooling::CPtr getCooling()
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

    sa::policies::IMoveSelector::CPtr getMoveSelector()
    {
      if (moveSelectorIndex == 0) {
        return std::make_unique<sa::policies::KBest>(k);
      }
      return nullptr;
    }

    sa::policies::IResource::CPtr getResource()
    {
      if (resourceIndex == 0) {
        return std::make_unique<sa::policies::Time>(durationInSeconds);
      }
      if (resourceIndex == 1) {
        return std::make_unique<sa::policies::Iteration>(iteration);
      }
      return nullptr;
    }

    sa::monitor::Monitor::CPtr getMonitor()
    {
      if (monitorIndex == 0) {
        return std::make_unique<sa::monitor::Monitor>(sa::monitor::MonitorLevel::Low, bestCatchQ);
      }
      if (monitorIndex == 1) {
        return std::make_unique<sa::monitor::Monitor>(sa::monitor::MonitorLevel::Medium, bestCatchQ, 1e-6,
                                                      localEnvLength, steps, memoryLimitInGb * 1000000000UL);
      }
      if (monitorIndex == 2) {
        return std::make_unique<sa::monitor::Monitor>(sa::monitor::MonitorLevel::High, bestCatchQ, 1e-6, localEnvLength,
                                                      20, memoryLimitInGb * 1000000000UL);
      }
      return nullptr;
    }

    sa::sa::SAFactory::CPtr getFactory()
    {
      return std::make_unique<sa::sa::SAFactory>(getResource(), getAcceptance(), getCooling(), getMoveSelector(),
                                                 getMonitor());
    }
  };

  void saFactoryUpdate()
  {
    ImGui::PushItemWidth(ImGui::GetWindowHeight() * 0.15f);

    combo("Resource Policy:", "##ResourcePolicy", &loadingSAFactoryParams.resourceIndex, resourceNames);
    ImGui::SameLine();
    if (loadingSAFactoryParams.resourceIndex == 0) {
      readDoubleNonNeg("Duration(s):", "##DurationInSecondsInput", &loadingSAFactoryParams.durationInSeconds);
    }
    if (loadingSAFactoryParams.resourceIndex == 1) {
      readU64("Iteration:", "##IterationInput", &loadingSAFactoryParams.iteration);
    }

    combo("Acceptance Policy:", "##AcceptancePolicy", &loadingSAFactoryParams.acceptanceIndex, acceptanceNames);
    if (loadingSAFactoryParams.acceptanceIndex == 0) {
      ImGui::SameLine();
      readDoubleNonNeg("Normalizator:", "##NormalizatorInput", &loadingSAFactoryParams.normalizator);
    }

    combo("Cooling Policy:", "##CoolingPolicy", &loadingSAFactoryParams.coolingIndex, coolingNames);
    if (loadingSAFactoryParams.coolingIndex == 3 || loadingSAFactoryParams.coolingIndex == 4) {
      ImGui::SameLine();
      readDoubleNonNeg("C:", "##CInput", &loadingSAFactoryParams.c);
    }
    ImGui::SameLine();
    readDoubleNonNeg("T0:", "##T0Input", &loadingSAFactoryParams.t0);

    combo("Move Selector Policy:", "##MoveSelectorPolicy", &loadingSAFactoryParams.moveSelectorIndex,
          moveSelectorNames);
    ImGui::SameLine();
    readU32("K:", "##KInput", &loadingSAFactoryParams.k);

    combo("Monitor:", "##MonitorLevel", &loadingSAFactoryParams.monitorIndex, monitorNames);
    if (loadingSAFactoryParams.monitorIndex == 0) {
      ImGui::SameLine();
      readDouble01("BestCatchQ:", "##BestQ0Input", &loadingSAFactoryParams.bestCatchQ);
    }
    if (loadingSAFactoryParams.monitorIndex == 1) {
      ImGui::SameLine();
      readU32("Snapshots:", "##Snapshots1Input", &loadingSAFactoryParams.steps);
      ImGui::SameLine();
      readU32("LocalEnvLength:", "##localEnvLength1Input", &loadingSAFactoryParams.localEnvLength);
      ImGui::SameLine();
      readDouble01("BestCatchQ:", "##BestQ1Input", &loadingSAFactoryParams.bestCatchQ);
      ImGui::SameLine();
      readU32("MemoryLimit(Gb):", "##MemoryLimit1Input", &loadingSAFactoryParams.memoryLimitInGb);
    }
    if (loadingSAFactoryParams.monitorIndex == 2) {
      ImGui::SameLine();
      readU32("LocalEnvLength:", "##localEnvLength2Input", &loadingSAFactoryParams.localEnvLength);
      ImGui::SameLine();
      readDouble01("BestCatchQ:", "##BestQ2Input", &loadingSAFactoryParams.bestCatchQ);
      ImGui::SameLine();
      readU32("MemoryLimit(Gb):", "##MemoryLimit2Input", &loadingSAFactoryParams.memoryLimitInGb);
    }
    ImGui::PopItemWidth();
  }

  std::vector<const char*> resourceNames{"Time", "Iteration"};
  std::vector<const char*> acceptanceNames{"Metropolis", "Greedy"};
  std::vector<const char*> coolingNames{"Linear", "Quadratic", "Cosine", "Exponential", "Logarithmic"};
  std::vector<const char*> moveSelectorNames{"KBest"};
  std::vector<const char*> monitorNames{"Low", "Medium", "High"};
  Params currentSAFactoryParams;
  Params loadingSAFactoryParams;
};

#endif  // SIMULATED_ANNEALING_GRAPHICS_SA_FACTORY_UI_H_
