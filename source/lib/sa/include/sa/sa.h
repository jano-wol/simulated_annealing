#ifndef SIMULATED_ANNEALING_SA_SA_H_
#define SIMULATED_ANNEALING_SA_SA_H_

#include <random>
#include <vector>

#include <policies/Acceptance.h>
#include <policies/Resource.h>
#include <sa/IMove.h>
#include <sa/IPosition.h>

namespace sa::sa
{

template <typename Resource = policies::Iteration, typename Acceptance = policies::Metropolis>
class SA
{
public:
  SA(Resource resourcePolicy_, Acceptance acceptancePolicy_)
      : resourcePolicy(std::move(resourcePolicy_)), acceptancePolicy(std::move(acceptancePolicy_))
  {}

  void anneal(std::shared_ptr<IPosition> startPosition, double temperature = 1.0)
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
        std::shared_ptr<IPosition> neighbour = nullptr;
        auto m = currPosition->getMove();
        auto energyOpt = currPosition->getEnergyFast(m, currEnergy);
        if (energyOpt) {
          energyCandidate = *energyOpt;
        } else {
          neighbour = currPosition->makeMove(m);
          energyCandidate = neighbour->getEnergy();
        }
        double delta = energyCandidate - currEnergy;
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
        temperature = (resourcePolicy.getLeft() / resourcePolicy.getAll());
        resourcePolicy.updateLeft();
        ++idx;
      }
    }
  }

  // Relevant for current annealing process
  std::shared_ptr<IPosition> currPosition;
  std::vector<std::shared_ptr<IMove>> moves;
  double currEnergy;
  double bestEnergy;
  std::size_t bestIdx;
  std::vector<double> energies;

  // Only relevant if restarts happen
  std::shared_ptr<IPosition> bestInit;
  std::vector<std::shared_ptr<IMove>> bestMoves;

  // Random generator
  std::mt19937 mt;
  std::uniform_real_distribution<> dist;

  // Diagnostics fir current annealing process
  int downEnergyChanges;
  int upEnergyChanges;

private:
  Resource resourcePolicy;
  Acceptance acceptancePolicy;
};

}  // namespace sa::sa

#endif  // SIMULATED_ANNEALING_SA_SA_H_
