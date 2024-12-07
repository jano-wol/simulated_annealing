#ifndef SIMULATED_ANNEALING_SA_SA_H_
#define SIMULATED_ANNEALING_SA_SA_H_

#include <limits>
#include <sstream>
#include <string>
#include <vector>

#include <core/IMove.h>
#include <core/IPosition.h>
#include <core/MoveCandidate.h>
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
      : currEnergy(std::numeric_limits<double>::max()),
        currPosition(nullptr),
        resourcePolicy(std::move(resourcePolicy_)),
        acceptancePolicy(std::move(acceptancePolicy_)),
        coolingPolicy(std::move(coolingPolicy_)),
        moveSelectorPolicy(std::move(moveSelectorPolicy_)),
        monitor(std::move(monitor_))
  {}

  void anneal(const core::IPosition::CPtr& startPosition)
  {
    currEnergy = startPosition->getEnergy();
    currPosition = startPosition->clone();
    while (resourcePolicy->getLeft() > 0) {
      auto moveCandidate = moveSelectorPolicy->selectMove(currPosition, currEnergy);
      double progress = 1.0 - (resourcePolicy->getLeft() / resourcePolicy->getAll());
      double temperature = coolingPolicy->getTemperature(progress);
      monitor.candidatePhase();
      if (acceptancePolicy->accept(currEnergy, moveCandidate.delta, temperature)) {
        monitor.acceptancePhase();
        if (moveCandidate.neighbour == nullptr) {
          currPosition->makeMove(moveCandidate.move);
        } else {
          currPosition = std::move(moveCandidate.neighbour);
        }
        currEnergy = moveCandidate.energyCandidate;
      }
      resourcePolicy->updateLeft();
    }
  }

  std::string toString() const
  {
    std::stringstream ss;
    ss << "<" << resourcePolicy->toString() << ";" << acceptancePolicy->toString() << ";" << coolingPolicy->toString()
       << ";" << moveSelectorPolicy->toString() << ";" << monitor.toString() << ">";
    return ss.str();
  }

private:
  double currEnergy;
  core::IPosition::CPtr currPosition;
  policies::IResource::CPtr resourcePolicy;
  policies::IAcceptance::CPtr acceptancePolicy;
  policies::ICooling::CPtr coolingPolicy;
  policies::IMoveSelector::CPtr moveSelectorPolicy;
  monitor::Monitor monitor;
};

}  // namespace sa::sa

#endif  // SIMULATED_ANNEALING_SA_SA_H_
