#ifndef SIMULATED_ANNEALING_GRAPHICS_SA_FACTORY_PARAMS_H_
#define SIMULATED_ANNEALING_GRAPHICS_SA_FACTORY_PARAMS_H_

#include <cmath>
#include <cstddef>

#include <monitor/Monitor.h>
#include <policies/Acceptance.h>
#include <policies/Cooling.h>
#include <policies/MoveSelector.h>
#include <policies/Resource.h>
#include <sa/SAFactory.h>

class SAFactoryParams
{
public:
  SAFactoryParams& operator=(const SAFactoryParams& other)
  {
    if (this != &other) {
      currentResourceIndex = other.currentResourceIndex;
      durationInSeconds = other.durationInSeconds;
      iteration = other.iteration;

      currentAcceptanceIndex = other.currentAcceptanceIndex;
      normalizator = other.normalizator;

      currentCoolingIndex = other.currentCoolingIndex;
      c = other.c;
      t0 = other.t0;

      currentMoveSelectorIndex = other.currentMoveSelectorIndex;
      k = other.k;

      currentMonitorIndex = other.currentMonitorIndex;
      bestCatchQ = other.bestCatchQ;
      localEnvLength = other.localEnvLength;
      steps = other.steps;
      memoryLimitInGb = other.memoryLimitInGb;
    }
    return *this;
  }

  static constexpr double epsilon = 1e-6;
  static bool almostEqual(double a, double b, double tolerance = epsilon) { return std::abs(a - b) <= tolerance; }

  int currentResourceIndex = 0;
  double durationInSeconds = 5.0;
  std::size_t iteration = 1000000;

  int currentAcceptanceIndex = 0;
  double normalizator = 5.0;

  int currentCoolingIndex = 0;
  double c = 1.0;
  double t0 = 1.0;

  int currentMoveSelectorIndex = 0;
  int k = 1;

  int currentMonitorIndex = 1;
  double bestCatchQ = 0.9;
  int localEnvLength = 1000;
  int steps = 20;
  int memoryLimitInGb = 2;

  bool operator==(const SAFactoryParams& other) const
  {
    return currentResourceIndex == other.currentResourceIndex &&
           almostEqual(durationInSeconds, other.durationInSeconds) && iteration == other.iteration &&
           currentAcceptanceIndex == other.currentAcceptanceIndex && almostEqual(normalizator, other.normalizator) &&
           currentCoolingIndex == other.currentCoolingIndex && almostEqual(c, other.c) && almostEqual(t0, other.t0) &&
           currentMoveSelectorIndex == other.currentMoveSelectorIndex && k == other.k &&
           currentMonitorIndex == other.currentMonitorIndex && almostEqual(bestCatchQ, other.bestCatchQ) &&
           localEnvLength == other.localEnvLength && steps == other.steps && memoryLimitInGb == other.memoryLimitInGb;
  }

  bool operator!=(const SAFactoryParams& other) const { return !(*this == other); }

  sa::policies::IAcceptance::CPtr getAcceptance()
  {
    if (currentAcceptanceIndex == 0) {
      return std::make_unique<sa::policies::Metropolis>(normalizator);
    }
    if (currentAcceptanceIndex == 1) {
      return std::make_unique<sa::policies::Greedy>();
    }
    return nullptr;
  }

  sa::policies::ICooling::CPtr getCooling()
  {
    if (currentCoolingIndex == 0) {
      return std::make_unique<sa::policies::Linear>(t0);
    }
    if (currentCoolingIndex == 1) {
      return std::make_unique<sa::policies::Quadratic>(t0);
    }
    if (currentCoolingIndex == 2) {
      return std::make_unique<sa::policies::Cosine>(t0);
    }
    if (currentCoolingIndex == 3) {
      return std::make_unique<sa::policies::Exponential>(c, t0);
    }
    if (currentCoolingIndex == 4) {
      return std::make_unique<sa::policies::Logarithmic>(c, t0);
    }
    return nullptr;
  }

  sa::policies::IMoveSelector::CPtr getMoveSelector()
  {
    if (currentMoveSelectorIndex == 0) {
      return std::make_unique<sa::policies::KBest>(k);
    }
    return nullptr;
  }

  sa::policies::IResource::CPtr getResource()
  {
    if (currentResourceIndex == 0) {
      return std::make_unique<sa::policies::Time>(durationInSeconds);
    }
    if (currentResourceIndex == 1) {
      return std::make_unique<sa::policies::Iteration>(iteration);
    }
    return nullptr;
  }

  sa::monitor::Monitor::CPtr getMonitor()
  {
    if (currentMonitorIndex == 0) {
      return std::make_unique<sa::monitor::Monitor>(sa::monitor::MonitorLevel::Low, bestCatchQ);
    }
    if (currentMonitorIndex == 1) {
      return std::make_unique<sa::monitor::Monitor>(sa::monitor::MonitorLevel::Medium, bestCatchQ, 1e-6, localEnvLength,
                                                    steps, memoryLimitInGb * 1000000000UL);
    }
    if (currentMonitorIndex == 2) {
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

#endif  // SIMULATED_ANNEALING_GRAPHICS_SA_FACTORY_PARAMS_H_
