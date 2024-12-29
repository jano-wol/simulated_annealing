#ifndef SIMULATED_ANNEALING_CORE_RANDOM_H_
#define SIMULATED_ANNEALING_CORE_RANDOM_H_

#include <random>

namespace sa::core
{
class Random
{
public:
  Random() : r(seed) {}
  Random(int s) : r(s) {}
  Random(const Random& other) : r(other.r) {}
  Random(Random&& other) noexcept : r(std::move(other.r)) {}
  Random& operator=(const Random& other)
  {
    if (this != &other) {
      r = other.r;
    }
    return *this;
  }
  Random& operator=(Random&& other) noexcept
  {
    if (this != &other) {
      r = std::move(other.r);
    }
    return *this;
  }

  int randomInt(int min, int max) const;
  double randomDouble(double min, double max) const;
  static void setSeed(int s);

//private:
  mutable std::mt19937 r;

  static int seed;
};
}  // namespace sa::core

#endif  // SIMULATED_ANNEALING_CORE_RANDOM_H_
