#include <core/sa.h>
#include <cmath>

SA::SA()
{
  std::random_device rd;
  std::mt19937 mt;
  dist = std::uniform_real_distribution<double>(0.0, 1.0);
}

void SA::anneal(std::shared_ptr<IPosition> startPosition, int iterations, double temperature, double energyNormalizator)
{
  currEnergy = startPosition->getEnergy();
  bestEnergy = currEnergy;
  currPosition = startPosition;
  bestIdx = 0;
  energies.push_back(bestEnergy);
  downEnergyChanges = 0;
  upEnergyChanges = 0;

  for (size_t idx = 1; idx <= iterations; ++idx) {
    auto m = currPosition->getMove();
    auto neighbour = currPosition->makeMove(m);
    double energyCandidate = neighbour->getEnergy();
    double threshold = std::exp((-(energyCandidate - currEnergy) / energyNormalizator) / temperature);
    double randomResult = dist(mt);
    if (randomResult < threshold) {
      if (currEnergy < energyCandidate) {
        ++upEnergyChanges;
      } else {
        ++downEnergyChanges;
      }
      currPosition = neighbour;
      moves.push_back(m);
      currEnergy = energyCandidate;
      energies.push_back(currEnergy);
      if (currEnergy < bestEnergy) {
        bestEnergy = currEnergy;
        bestIdx = idx;
      }
    }
    temperature -= 1.0 / double(iterations);
  }
}
