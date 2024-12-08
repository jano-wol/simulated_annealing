#ifndef SIMULATED_ANNEALING_POLICIES_RESOURCE_H_
#define SIMULATED_ANNEALING_POLICIES_RESOURCE_H_

#include <chrono>
#include <memory>
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

  double getAll() const override;
  double getLeft() const override;
  void updateLeft();
  std::string toString() const override;

private:
  double all;
  double left;
};

class Time final : public IResource
{
public:
  Time(double allInSeconds, int quant_ = 20)
      : all(std::chrono::duration<double>(allInSeconds)),
        startTime(std::chrono::high_resolution_clock::now()),
        quant(quant_)
  {}

  double getAll() const override;
  double getLeft() const override;
  void updateLeft() override;
  std::string toString() const override;

private:
  std::chrono::duration<double> all;
  std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
  int quant;
  mutable int state = 0;
  mutable double lastRet = 0;
};
}  // namespace sa::policies

#endif  // SIMULATED_ANNEALING_POLICIES_RESOURCE_H_