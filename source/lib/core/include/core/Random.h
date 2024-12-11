#ifndef SIMULATED_ANNEALING_CORE_RANDOM_H_
#define SIMULATED_ANNEALING_CORE_RANDOM_H_

#include <random>

namespace sa::core
{
class Random
{
public:
  static int randomInt(int min, int max);
  static double randomDouble(double min, double max);
  static void timeInit();

private:
  static std::mt19937 createThreadLocalGenerator();
  static std::mt19937& getThreadLocalGenerator();
};
}  // namespace sa::core

#endif  // SIMULATED_ANNEALING_CORE_RANDOM_H_
