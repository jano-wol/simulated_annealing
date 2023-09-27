#include <core/sa.h>
#include <cmath>

SA::SA(int iterations_, std::shared_ptr<IPosition> start_) : iterations(iterations_), start(std::move(start_))
{
  temperature = 1.0;
  currEnergy = start->getEnergy();
  bestEnergy = currEnergy;
  curr = start;
  bestIdx = 0;
  energies.push_back(bestEnergy);
  std::random_device rd;
  std::mt19937 mt;
  dist = std::uniform_real_distribution<double>(0.0, 1.0);
}

void SA::anneal()
{
  for (size_t idx = 1; idx <= iterations; ++idx) {
    auto m = curr->getMove();
    auto neighbour = curr->makeMove(m);
    double energyCandidate = neighbour->getEnergy();
    double threshold = std::exp(-(energyCandidate - currEnergy) / temperature);
    double ran = dist(mt);
    if (ran < threshold) {
      curr = neighbour;
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
