#ifndef SIMULATED_ANNEALING_SA_SA_H_
#define SIMULATED_ANNEALING_SA_SA_H_

#include <sstream>
#include <string>
#include <vector>

#include <core/IMove.h>
#include <core/IPosition.h>
#include <policies/Acceptance.h>
#include <policies/Cooling.h>
#include <policies/Resource.h>

namespace sa::sa
{
class SA
{
public:
  SA(policies::IResource::CPtr resourcePolicy_, policies::IAcceptance::CPtr acceptancePolicy_,
     policies::ICooling::CPtr coolingPolicy_)
      : resourcePolicy(std::move(resourcePolicy_)),
        acceptancePolicy(std::move(acceptancePolicy_)),
        coolingPolicy(std::move(coolingPolicy_))
  {}

  void anneal(const core::IPosition::CPtr& startPosition)
  {
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
        double delta;
        double energyCandidate;
        auto m = currPosition->generateMove();
        core::IPosition::CPtr neighbour;

        auto deltaOpt = currPosition->getDelta(m);
        if (!deltaOpt) {
          neighbour = currPosition->createNeighbour(m);
          energyCandidate = neighbour->getEnergy();
          delta = energyCandidate - currEnergy;
        } else {
          delta = *deltaOpt;
          energyCandidate = currEnergy + delta;
        }
        double progress = 1.0 - (resourcePolicy->getLeft() / resourcePolicy->getAll());
        double temperature = coolingPolicy->getTemperature(progress);
        if (acceptancePolicy->accept(currEnergy, delta, temperature)) {
          if (currEnergy < energyCandidate) {
            ++upEnergyChanges;
          } else {
            ++downEnergyChanges;
          }
          if (neighbour == nullptr) {
            currPosition->makeMove(m);
          } else {
            currPosition = std::move(neighbour);
          }
          moves.push_back(m);
          currEnergy = energyCandidate;
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
  }

  std::string toString() const
  {
    std::stringstream ss;
    ss << "<" << resourcePolicy->toString() << ";" << acceptancePolicy->toString() << ";" << coolingPolicy->toString()
       << ">";
    return ss.str();
  }

  // Relevant for current annealing process
  core::IPosition::CPtr currPosition;
  std::vector<core::IMove::Ptr> moves;
  double currEnergy;
  double bestEnergy;
  std::size_t bestIdx;
  std::vector<double> energies;

  // Only relevant if restarts happen
  core::IPosition::CPtr bestInit;
  std::vector<core::IMove::Ptr> bestMoves;

  // Diagnostics fir current annealing process
  int downEnergyChanges;
  int upEnergyChanges;

private:
  policies::IResource::CPtr resourcePolicy;
  policies::IAcceptance::CPtr acceptancePolicy;
  policies::ICooling::CPtr coolingPolicy;
};

}  // namespace sa::sa

#endif  // SIMULATED_ANNEALING_SA_SA_H_
