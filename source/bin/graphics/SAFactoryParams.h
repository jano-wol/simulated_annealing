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
  double normalizator = 5.0;

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

  bool operator==(const SAFactoryParams& other) const
  {
    return resourceIndex == other.resourceIndex && almostEqual(durationInSeconds, other.durationInSeconds) &&
           iteration == other.iteration && acceptanceIndex == other.acceptanceIndex &&
           almostEqual(normalizator, other.normalizator) && coolingIndex == other.coolingIndex &&
           almostEqual(c, other.c) && almostEqual(t0, other.t0) && moveSelectorIndex == other.moveSelectorIndex &&
           k == other.k && monitorIndex == other.monitorIndex && almostEqual(bestCatchQ, other.bestCatchQ) &&
           localEnvLength == other.localEnvLength && steps == other.steps && memoryLimitInGb == other.memoryLimitInGb;
  }

  bool operator!=(const SAFactoryParams& other) const { return !(*this == other); }

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
      return std::make_unique<sa::monitor::Monitor>(sa::monitor::MonitorLevel::Medium, bestCatchQ, 1e-6, localEnvLength,
                                                    steps, memoryLimitInGb * 1000000000UL);
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

#endif  // SIMULATED_ANNEALING_GRAPHICS_SA_FACTORY_PARAMS_H_
