#ifndef SIMULATED_ANNEALING_SA_SA_H_
#define SIMULATED_ANNEALING_SA_SA_H_

#include <string>

#include <core/IMove.h>
#include <core/IPosition.h>
#include <monitor/Monitor.h>
#include <policies/Acceptance.h>
#include <policies/Cooling.h>
#include <policies/MoveSelector.h>
#include <policies/Resource.h>

namespace sa::sa
{
class SA
{
public:
  using CPtr = std::unique_ptr<SA>;
  using StopCallback = std::function<bool(void)>;
  SA(
      policies::IResource::CPtr resourcePolicy_, policies::IAcceptance::CPtr acceptancePolicy_,
      policies::ICooling::CPtr coolingPolicy_, policies::IMoveSelector::CPtr moveSelectorPolicy_,
      monitor::Monitor::CPtr monitor_, StopCallback stopCallback_ = []() { return false; })
      : resourcePolicy(std::move(resourcePolicy_)),
        acceptancePolicy(std::move(acceptancePolicy_)),
        coolingPolicy(std::move(coolingPolicy_)),
        moveSelectorPolicy(std::move(moveSelectorPolicy_)),
        monitor(std::move(monitor_)),
        stopCallback(std::move(stopCallback_))
  {}

  void anneal(const core::IPosition::CPtr& startPosition);
  const core::IPosition::CPtr& getBest();
  std::string toString() const;

  policies::IResource::CPtr resourcePolicy;
  policies::IAcceptance::CPtr acceptancePolicy;
  policies::ICooling::CPtr coolingPolicy;
  policies::IMoveSelector::CPtr moveSelectorPolicy;
  monitor::Monitor::CPtr monitor;
  StopCallback stopCallback;
  core::IPosition::CPtr currPosition;
};
}  // namespace sa::sa

#endif  // SIMULATED_ANNEALING_SA_SA_H_
