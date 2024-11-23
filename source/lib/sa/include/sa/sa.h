#ifndef SIMULATED_ANNEALING_SA_SA_H_
#define SIMULATED_ANNEALING_SA_SA_H_

#include <sa/IMove.h>
#include <sa/IPosition.h>
#include <random>
#include <vector>

namespace sa::sa
{

template <typename ResourcePolicy>
class SA
{
public:
  SA::SA()
  {
    mt = std::mt19937(0);
    dist = std::uniform_real_distribution<double>(0.0, 1.0);
  }

  void anneal(std::shared_ptr<IPosition> startPosition, std::size_t iterations, double temperature = 1.0,
              double energyNormalizator = 1.0)
  {
    {
      currEnergy = startPosition->getEnergy();
      bestEnergy = currEnergy;
      currPosition = startPosition;
      bestIdx = 0;
      energies.push_back(bestEnergy);
      downEnergyChanges = 0;
      upEnergyChanges = 0;

      for (std::size_t idx = 1; idx <= iterations; ++idx) {
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
        double threshold = std::exp((-(energyCandidate - currEnergy) / energyNormalizator) / temperature);
        double randomResult = dist(mt);
        if (randomResult < threshold) {
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
        temperature -= 1.0 / double(iterations);
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
  ResourcePolicy resourcePolicy;
};

}  // namespace sa::sa

#endif  // SIMULATED_ANNEALING_SA_SA_H_
