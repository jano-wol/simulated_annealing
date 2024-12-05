#ifndef SIMULATED_ANNEALING_SA_SA_H_
#define SIMULATED_ANNEALING_SA_SA_H_

#include <sstream>
#include <string>
#include <vector>

#include <core/IMove.h>
#include <core/IPosition.h>
#include <core/MoveCandidate.h>
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
     policies::ICooling::CPtr coolingPolicy_, policies::IMoveSelector::CPtr moveSelectorPolicy_)
      : resourcePolicy(std::move(resourcePolicy_)),
        acceptancePolicy(std::move(acceptancePolicy_)),
        coolingPolicy(std::move(coolingPolicy_)),
        moveSelectorPolicy(std::move(moveSelectorPolicy_))
  {}

  void anneal(const core::IPosition::CPtr& startPosition)
  {
    currEnergy = startPosition->getEnergy();
    bestEnergy = currEnergy;
    currPosition = startPosition->clone();
    bestIdx = 0;
    energies.push_back(bestEnergy);
    downEnergyChanges = 0;
    upEnergyChanges = 0;
    size_t idx = 0;
    while (resourcePolicy->getLeft() > 0) {
      auto moveCandidate = moveSelectorPolicy->selectMove(currPosition, currEnergy);
      double progress = 1.0 - (resourcePolicy->getLeft() / resourcePolicy->getAll());
      double temperature = coolingPolicy->getTemperature(progress);
      if (acceptancePolicy->accept(currEnergy, moveCandidate.delta, temperature)) {
        if (currEnergy < moveCandidate.energyCandidate) {
          ++upEnergyChanges;
        } else {
          ++downEnergyChanges;
        }
        if (moveCandidate.neighbour == nullptr) {
          currPosition->makeMove(moveCandidate.move);
        } else {
          currPosition = std::move(moveCandidate.neighbour);
        }
        moves.push_back(std::move(moveCandidate.move));
        currEnergy = moveCandidate.energyCandidate;
        energies.push_back(currEnergy);
        if (currEnergy <= bestEnergy) {
          bestEnergy = currEnergy;
          bestIdx = idx;
        }
      }
      resourcePolicy->updateLeft();
      ++idx;
    }
  }

  std::string toString() const
  {
    std::stringstream ss;
    ss << "<" << resourcePolicy->toString() << ";" << acceptancePolicy->toString() << ";" << coolingPolicy->toString()
       << ";" << moveSelectorPolicy->toString() << ">";
    return ss.str();
  }

  // Relevant for current annealing process
  core::IPosition::CPtr currPosition;
  std::vector<core::IMove::CPtr> moves;
  double currEnergy;
  double bestEnergy;
  std::size_t bestIdx;
  std::vector<double> energies;

  // Only relevant if restarts happen
  core::IPosition::CPtr bestInit;
  std::vector<core::IMove::CPtr> bestMoves;

  // Diagnostics fir current annealing process
  int downEnergyChanges;
  int upEnergyChanges;

private:
  policies::IResource::CPtr resourcePolicy;
  policies::IAcceptance::CPtr acceptancePolicy;
  policies::ICooling::CPtr coolingPolicy;
  policies::IMoveSelector::CPtr moveSelectorPolicy;
};

}  // namespace sa::sa

#endif  // SIMULATED_ANNEALING_SA_SA_H_
