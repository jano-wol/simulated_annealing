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
  SA(policies::IResource::CPtr resourcePolicy_, policies::IAcceptance::CPtr acceptancePolicy_,
     policies::ICooling::CPtr coolingPolicy_, policies::IMoveSelector::CPtr moveSelectorPolicy_,
     monitor::Monitor monitor_)
      : currPosition(nullptr),
        resourcePolicy(std::move(resourcePolicy_)),
        acceptancePolicy(std::move(acceptancePolicy_)),
        coolingPolicy(std::move(coolingPolicy_)),
        moveSelectorPolicy(std::move(moveSelectorPolicy_)),
        monitor(std::move(monitor_))
  {}

  void anneal(const core::IPosition::CPtr& startPosition);
  std::string toString() const;

  core::IPosition::CPtr currPosition;
  policies::IResource::CPtr resourcePolicy;
  policies::IAcceptance::CPtr acceptancePolicy;
  policies::ICooling::CPtr coolingPolicy;
  policies::IMoveSelector::CPtr moveSelectorPolicy;
  monitor::Monitor monitor;
};
}  // namespace sa::sa

#endif  // SIMULATED_ANNEALING_SA_SA_H_
