#ifndef SIMULATED_ANNEALING_POLICIES_RESOURCE_H_
#define SIMULATED_ANNEALING_POLICIES_RESOURCE_H_

#include <algorithm>
#include <chrono>
#include <memory>
#include <sstream>
#include <string>

namespace sa::policies
{
class IResource
{
public:
  using CPtr = std::unique_ptr<IResource>;
  virtual double getAll() const = 0;
  virtual double getLeft() const = 0;
  virtual void updateLeft() = 0;
  virtual std::string toString() const = 0;
  virtual ~IResource() = default;
};

class Iteration final : public IResource
{
public:
  Iteration(double all_) : all(all_), left(all_) {}

  double getAll() const override { return all; }

  double getLeft() const override { return left; }

  void updateLeft() override
  {
    if (left > 0) {
      --left;
    }
  }
  std::string toString() const override
  {
    std::stringstream ss;
    ss << "Iteration=" << std::size_t(all);
    return ss.str();
  }

private:
  double all;
  double left;
};

class Time final : public IResource
{
public:
  Time(double allInSeconds)
      : all(std::chrono::duration<double>(allInSeconds)), startTime(std::chrono::high_resolution_clock::now())
  {}

  double getAll() const override { return all.count(); }

  double getLeft() const override
  {
    auto elapsed = std::chrono::high_resolution_clock::now() - startTime;
    double elapsedSeconds = std::chrono::duration<double>(elapsed).count();
    double ret = std::max(0.0, all.count() - elapsedSeconds);
    return ret;
  }

  void updateLeft() override {}

  std::string toString() const override
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