#ifndef SIMULATED_ANNEALING_POLICIES_RESOURCE_H_
#define SIMULATED_ANNEALING_POLICIES_RESOURCE_H_

#include <algorithm>
#include <chrono>
#include <sstream>
#include <string>

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
  std::string toString() const
  {
    std::stringstream ss;
    ss << "Iteration=" << std::size_t(all);
    return ss.str();
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
    double elapsedSeconds = std::chrono::duration<double>(elapsed).count();
    double ret = std::max(0.0, all.count() - elapsedSeconds);
    return ret;
  }

  void updateLeft() {}

  std::string toString() const
  {
    std::stringstream ss;
    ss << "Time=" << std::size_t(all.count()) << "s";
    return ss.str();
  }

private:
  std::chrono::duration<double> all;
  std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
};

}  // namespace sa::policies

#endif  // SIMULATED_ANNEALING_POLICIES_RESOURCE_H_