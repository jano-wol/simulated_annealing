#ifndef SIMULATED_ANNEALING_POLICIES_ACCEPTANCE_H_
#define SIMULATED_ANNEALING_POLICIES_ACCEPTANCE_H_

#include <memory>
#include <string>

namespace sa::policies
{
class IAcceptance
{
public:
  using CPtr = std::unique_ptr<IAcceptance>;
  virtual bool accept(double currEnergy, double delta, double temperature) const = 0;
  virtual std::string toString() const = 0;
  virtual ~IAcceptance() = default;
};

class Metropolis final : public IAcceptance
{
public:
  Metropolis(double normalizator_ = 1.0) : normalizator(normalizator_) {}

  bool accept(double /*currEnergy*/, double delta, double temperature) const override;
  std::string toString() const override;

  double normalizator;
};

class Greedy final : public IAcceptance
{
public:
  Greedy() {}

  bool accept(double /*currEnergy*/, double delta, double /*temperature*/) const override;
  std::string toString() const override;
};
}  // namespace sa::policies

#endif  // SIMULATED_ANNEALING_POLICIES_ACCEPTANCE_H_