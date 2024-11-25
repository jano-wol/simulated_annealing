#ifndef SIMULATED_ANNEALING_POLICIES_ACCEPTANCE_H_
#define SIMULATED_ANNEALING_POLICIES_ACCEPTANCE_H_

#include <algorithm>
#include <cmath>
#include <random>
#include <sstream>
#include <string>

namespace sa::policies
{

class Metropolis
{
public:
  Metropolis(double normalizator_ = 1.0) : normalizator(normalizator_)
  {
    mt = std::mt19937(0);
    dist = std::uniform_real_distribution<double>(0.0, 1.0);
  }

  bool accept(double /*currEnergy*/, double delta, double temperature)
  {
    if (delta <= 0)
      return true;
    double threshold = std::exp(-delta / normalizator / temperature);
    double randomResult = dist(mt);
    return randomResult < threshold;
  }

  std::string toString() const
  {
    std::stringstream ss;
    ss << "Metropolis";
    if (normalizator != 1) {
      ss << "[norm=" << normalizator << "]";
    }
    return ss.str();
  }

  std::mt19937 mt;
  std::uniform_real_distribution<> dist;
  double normalizator;
};

class Greedy
{
public:
  Greedy() {}

  bool accept(double /*currEnergy*/, double delta, double /*temperature*/) const { return delta <= 0; }

  std::string toString() const { return "Greedy"; }
};

}  // namespace sa::policies

#endif  // SIMULATED_ANNEALING_POLICIES_ACCEPTANCE_H_