#include <core/sa.h>
#include <cmath>

using namespace sa::core;

SA::SA()
{
  std::random_device rd;
  mt = std::mt19937(rd);
  dist = std::uniform_real_distribution<double>(0.0, 1.0);
}

void SA::anneal(std::shared_ptr<IPosition> startPosition, std::size_t iterations, double temperature,
                double energyNormalizator)
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
