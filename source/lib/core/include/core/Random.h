#ifndef SIMULATED_ANNEALING_CORE_RANDOM_H_
#define SIMULATED_ANNEALING_CORE_RANDOM_H_

#include <chrono>
#include <random>
#include <thread>

namespace sa::core
{
class Random
{
public:
  static int randomInt(int min, int max)
  {
    std::uniform_int_distribution<int> dist(min, max);
    return dist(getThreadLocalGenerator());
  }

  static double randomDouble(double min, double max)
  {
    std::uniform_real_distribution<double> dist(min, max);
    return dist(getThreadLocalGenerator());
  }

  static void timeInit()
  {
    auto now = std::chrono::steady_clock::now();
    auto time_since_epoch = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
    getThreadLocalGenerator() = std::mt19937(static_cast<unsigned int>(time_since_epoch));
  }

private:
  static std::mt19937 createThreadLocalGenerator()
  {
    auto threadIdHash = std::hash<std::thread::id>()(std::this_thread::get_id());
    return std::mt19937(static_cast<unsigned int>(threadIdHash));
  }

  static std::mt19937& getThreadLocalGenerator()
  {
    thread_local std::mt19937 generator = createThreadLocalGenerator();
    return generator;
  }
};
}  // namespace sa::core

#endif  // SIMULATED_ANNEALING_CORE_RANDOM_H_
