#ifndef SIMULATED_ANNEALING_SA_SA_H_
#define SIMULATED_ANNEALING_SA_SA_H_

#include <random>
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

template <typename Resource = policies::Iteration, typename Acceptance = policies::Metropolis,
          typename Cooling = policies::Linear>
class SA
{
public:
  SA(Resource resourcePolicy_, Acceptance acceptancePolicy_, Cooling coolingPolicy_)
      : resourcePolicy(std::move(resourcePolicy_)),
        acceptancePolicy(std::move(acceptancePolicy_)),
        coolingPolicy(std::move(coolingPolicy_))
  {}

  void anneal(std::shared_ptr<core::IPosition> startPosition)
  {
    {
      currEnergy = startPosition->getEnergy();
      bestEnergy = currEnergy;
      currPosition = startPosition;
      bestIdx = 0;
      energies.push_back(bestEnergy);
      downEnergyChanges = 0;
      upEnergyChanges = 0;
      size_t idx = 0;
      while (resourcePolicy.getLeft() > 0) {
        double energyCandidate;
        std::shared_ptr<core::IPosition> neighbour = nullptr;
        auto m = currPosition->getMove();
        auto energyOpt = currPosition->getEnergyFast(m, currEnergy);
        if (energyOpt) {
          energyCandidate = *energyOpt;
        } else {
          neighbour = currPosition->makeMove(m);
          energyCandidate = neighbour->getEnergy();
        }
        double delta = energyCandidate - currEnergy;
        double progress = 1.0 - (resourcePolicy.getLeft() / resourcePolicy.getAll());
        double temperature = coolingPolicy.getTemperature(progress);
        if (acceptancePolicy.accept(currEnergy, delta, temperature)) {
          if (currEnergy < energyCandidate) {
            ++upEnergyChanges;
          } else {
            ++downEnergyChanges;
          }
          if (neighbour == nullptr) {
            currPosition->makeMoveInplace(m);
          } else {
            currPosition = neighbour;
          }
          moves.push_back(m);
          currEnergy = energyCandidate;
          energies.push_back(currEnergy);
          if (currEnergy <= bestEnergy) {
            bestEnergy = currEnergy;
            bestIdx = idx;
          }
        }
        resourcePolicy.updateLeft();
        ++idx;
      }
    }
  }

  std::string toString() const
  {
    std::stringstream ss;
    ss << "<" << resourcePolicy.toString() << ";" << acceptancePolicy.toString() << ";" << coolingPolicy.toString()
       << ">";
    return ss.str();
  }

  // Relevant for current annealing process
  std::shared_ptr<core::IPosition> currPosition;
  std::vector<std::shared_ptr<core::IMove>> moves;
  double currEnergy;
  double bestEnergy;
  std::size_t bestIdx;
  std::vector<double> energies;

  // Only relevant if restarts happen
  std::shared_ptr<core::IPosition> bestInit;
  std::vector<std::shared_ptr<core::IMove>> bestMoves;

  // Random generator
  std::mt19937 mt;
  std::uniform_real_distribution<> dist;

  // Diagnostics fir current annealing process
  int downEnergyChanges;
  int upEnergyChanges;

private:
  Resource resourcePolicy;
  Acceptance acceptancePolicy;
  Cooling coolingPolicy;
};

}  // namespace sa::sa

#endif  // SIMULATED_ANNEALING_SA_SA_H_
