#ifndef SIMULATED_ANNEALING_POLICIES_COOLING_H_
#define SIMULATED_ANNEALING_POLICIES_COOLING_H_

#include <memory>
#include <string>

namespace sa::policies
{
class ICooling
{
public:
  using CPtr = std::unique_ptr<ICooling>;
  virtual double getTemperature(double progress) const = 0;
  virtual std::string toString() const = 0;
  virtual ~ICooling() = default;
};

class Linear final : public ICooling
{
public:
  Linear(double temperature_ = 1.0) : t0(temperature_) {}

  double getTemperature(double progress) const override;
  std::string toString() const override;

private:
  double t0;
};

class Exponential final : public ICooling
{
public:
  Exponential(double c_, double temperature_ = 1.0) : c(c_), t0(temperature_) {}

  double getTemperature(double progress) const override;
  std::string toString() const override;

private:
  double c;
  double t0;
};

class Logarithmic final : public ICooling
{
public:
  Logarithmic(double c_, double temperature_ = 1.0) : c(c_), t0(temperature_) {}

  double getTemperature(double progress) const override;
  std::string toString() const override;

private:
  double c;
  double t0;
};

class Quadratic final : public ICooling
{
public:
  Quadratic(double temperature_ = 1.0) : t0(temperature_) {}

  double getTemperature(double progress) const override;
  std::string toString() const override;

private:
  double t0;
};

class Cosine final : public ICooling
{
public:
  Cosine(double temperature_ = 1.0) : t0(temperature_) {}

  double getTemperature(double progress) const override;
  std::string toString() const override;

private:
  double t0;
};
}  // namespace sa::policies

#endif  // SIMULATED_ANNEALING_POLICIES_COOLING_H_