#include <core/IMove.h>
#include <core/IPosition.h>
#include <random>
#include <vector>

class SA
{
public:
  SA(int iterationsLeft_, std::shared_ptr<IPosition> start_);

  void anneal();
  int iterations;
  std::shared_ptr<IPosition> start;
  double temperature;
  double currEnergy;
  double bestEnergy;
  std::size_t bestIdx;
  std::shared_ptr<IPosition> curr;
  std::vector<std::shared_ptr<IMove>> moves;
  std::vector<double> energies;
  std::mt19937 mt;
  std::uniform_real_distribution<> dist;

  // only relevant if restarts happen
  std::shared_ptr<IPosition> bestInit;
  std::vector<std::shared_ptr<IMove>> bestMoves;
};