#include <core/IMove.h>
#include <core/IPosition.h>
#include <random>
#include <vector>

class SA
{
public:
  SA();

  void anneal(std::shared_ptr<IPosition> startPosition, int iterations, double temperature = 1.0,
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

  // Diagnostics
  int positionChanges;
  int upEnergyChanges;
};