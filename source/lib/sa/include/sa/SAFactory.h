#ifndef SIMULATED_ANNEALING_SA_SA_FACTORY_H_
#define SIMULATED_ANNEALING_SA_SA_FACTORY_H_

#include <sa/SA.h>

namespace sa::sa
{
class SAFactory
{
public:
  using CPtr = std::unique_ptr<SAFactory>;
  SAFactory(policies::IResource::CPtr resourcePolicy_, policies::IAcceptance::CPtr acceptancePolicy_,
            policies::ICooling::CPtr coolingPolicy_, policies::IMoveSelector::CPtr moveSelectorPolicy_,
            monitor::Monitor::CPtr monitor_)
      : resourcePolicy(std::move(resourcePolicy_)),
        acceptancePolicy(std::move(acceptancePolicy_)),
        coolingPolicy(std::move(coolingPolicy_)),
        moveSelectorPolicy(std::move(moveSelectorPolicy_)),
        monitor(std::move(monitor_))
  {}

  SA::CPtr create() const;

  policies::IResource::CPtr resourcePolicy;
  policies::IAcceptance::CPtr acceptancePolicy;
  policies::ICooling::CPtr coolingPolicy;
  policies::IMoveSelector::CPtr moveSelectorPolicy;
  monitor::Monitor::CPtr monitor;
};
}  // namespace sa::sa

#endif  // SIMULATED_ANNEALING_SA_SA_FACTORY_H_
