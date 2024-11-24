#ifndef SIMULATED_ANNEALING_POLICIES_ACCEPTANCE_H_
#define SIMULATED_ANNEALING_POLICIES_ACCEPTANCE_H_

#include <algorithm>
#include <cmath>
#include <random>

namespace sa::policies
{

class Metropolis
{
public:
  Metropolis()
  {
    mt = std::mt19937(0);
    dist = std::uniform_real_distribution<double>(0.0, 1.0);
  }

  bool accept(double /*currEnergy*/, double delta, double temperature)
  {
    if (delta <= 0)
      return true;
    double threshold = std::exp(-delta / temperature);
    double randomResult = dist(mt);
    return randomResult < threshold;
  }

  std::mt19937 mt;
  std::uniform_real_distribution<> dist;
};

class Greedy
{
public:
  Greedy() {}
  bool accept(double /*currEnergy*/, double delta, double /*temperature*/) const { return delta <= 0; }
};

}  // namespace sa::policies

#endif  // SIMULATED_ANNEALING_POLICIES_ACCEPTANCE_H_