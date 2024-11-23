#ifndef SIMULATED_ANNEALING_SA_SA_H_
#define SIMULATED_ANNEALING_SA_SA_H_

#include <sa/IMove.h>
#include <sa/IPosition.h>
#include <random>
#include <vector>

namespace sa::sa
{
class SA
{
public:
  SA();

  void anneal(std::shared_ptr<IPosition> startPosition, std::size_t iterations, double temperature = 1.0,
              double energyNormalizator = 1.0);

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
};
}  // namespace sa::sa

#endif  // SIMULATED_ANNEALING_SA_SA_H_
