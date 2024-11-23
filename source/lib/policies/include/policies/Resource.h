#ifndef SIMULATED_ANNEALING_POLICIES_RESOURCE_H_
#define SIMULATED_ANNEALING_POLICIES_RESOURCE_H_

#include <algorithm>
#include <chrono>

namespace sa::policies
{
class Iteration
{
public:
  Iteration(double all_) : all(all_), left(all_) {}
  double getAll() const { return all; }
  double getLeft() const { return left; }
  void updateLeft()
  {
    if (left > 0) {
      --left;
    }
  }

private:
  double all;
  double left;
};

class Time
{
public:
  Time(double allInSeconds)
      : all(std::chrono::duration<double>(allInSeconds)), startTime(std::chrono::high_resolution_clock::now())
  {}

  double getAll() const { return all.count(); }

  double getLeft() const
  {
    auto elapsed = std::chrono::high_resolution_clock::now() - startTime;
    return std::max(0.0, all.count() - elapsed.count());
  }

  void updateLeft() {}

private:
  std::chrono::duration<double> all;
  std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
};

}  // namespace sa::policies

#endif  // SIMULATED_ANNEALING_POLICIES_RESOURCE_H_