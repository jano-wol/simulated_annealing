#ifndef SIMULATED_ANNEALING_SA_STUDIO_SA_FACTORY_UI_H_
#define SIMULATED_ANNEALING_SA_STUDIO_SA_FACTORY_UI_H_

#include <algorithm>
#include <atomic>
#include <deque>
#include <thread>

#include <monitor/Monitor.h>
#include <policies/Acceptance.h>
#include <policies/Cooling.h>
#include <policies/MoveSelector.h>
#include <policies/Resource.h>
#include <sa/SAFactory.h>

class SAFactoryUI
{
public:
  class Params
  {
  public:
    int resourceIndex = 0;
    double durationInSeconds = 5.0;
    std::size_t iteration = 1000000;

    int acceptanceIndex = 0;

    int coolingIndex = 0;
    double t0 = 1.0;

    int moveSelectorIndex = 0;
    int k = 1;

    int monitorIndex = 0;
    int steps = 100;
    int memoryLimitInGb = 1;

    int randomIndex = 0;
    int seed = 0;

    int repeats = 1;
    int threads = 1;
    int threadsMaximum = std::thread::hardware_concurrency() > 1 ? std::thread::hardware_concurrency() - 1 : 1;

    sa::policies::IAcceptance::CPtr getAcceptance();
    sa::policies::ICooling::CPtr getCooling();
    sa::policies::IMoveSelector::CPtr getMoveSelector();
    sa::policies::IResource::CPtr getResource();
    sa::monitor::Monitor::CPtr getMonitor(std::atomic<double>& progress);
    sa::sa::SAFactory::CPtr getFactory(std::atomic<double>& progress, std::atomic<bool>& stop);
    std::vector<sa::sa::SAFactory::CPtr> getFactories(std::deque<std::atomic<double>>& progresses,
                                                      std::atomic<bool>& stop);
  };

  void saFactoryUpdate();
  void setRandomSeed();

  std::vector<const char*> resourceNames{"Time", "Iteration"};
  std::vector<const char*> acceptanceNames{"Metropolis", "Greedy"};
  std::vector<const char*> coolingNames{"Linear", "Quadratic", "Cosine"};
  std::vector<const char*> moveSelectorNames{"KBest"};
  std::vector<const char*> monitorNames{"Snapshots", "All"};
  std::vector<const char*> randomNames{"Time-based", "Explicit"};
  Params loadedParams;
};

#endif  // SIMULATED_ANNEALING_SA_STUDIO_SA_FACTORY_UI_H_
