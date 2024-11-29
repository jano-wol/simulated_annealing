#ifndef SIMULATED_ANNEALING_POLICIES_COOLING_H_
#define SIMULATED_ANNEALING_POLICIES_COOLING_H_

#include <algorithm>
#include <cmath>
#include <memory>
#include <sstream>
#include <string>

namespace sa::policies
{
class ICooling
{
public:
  using Ptr = std::shared_ptr<ICooling>;
  virtual double getTemperature(double progress) const = 0;
  virtual std::string toString() const = 0;
  virtual ~ICooling() = default;
};

class Linear final : public ICooling
{
public:
  Linear(double temperature_ = 1.0) : t0(temperature_) {}

  double getTemperature(double progress) const override { return t0 * (1.0 - progress); }

  std::string toString() const override
  {
    std::stringstream ss;
    ss << "Linear";
    if (t0 != 1) {
      ss << "[t0=" << t0 << "]";
    }
    return ss.str();
  }

private:
  double t0;
};

class Exponential final : public ICooling
{
public:
  Exponential(double c_, double temperature_ = 1.0) : c(c_), t0(temperature_) {}

  double getTemperature(double progress) const override { return t0 * std::exp(c * progress); }

  std::string toString() const override
  {
    std::stringstream ss;
    ss << "Exponential[c=" << c;
    if (t0 != 1) {
      ss << ";t0=" << t0;
    }
    ss << "]";
    return ss.str();
  }

private:
  double c;
  double t0;
};

class Logarithmic final : public ICooling
{
public:
  Logarithmic(double c_, double temperature_ = 1.0) : c(c_), t0(temperature_) {}

  double getTemperature(double progress) const override { return t0 / (1 + c * std::log(1.0 + progress)); }

  std::string toString() const override
  {
    std::stringstream ss;
    ss << "Logarithmic[c=" << c;
    if (t0 != 1) {
      ss << ";t0=" << t0;
    }
    ss << "]";
    return ss.str();
  }

private:
  double c;
  double t0;
};

class Quadratic final : public ICooling
{
public:
  Quadratic(double temperature_ = 1.0) : t0(temperature_) {}

  double getTemperature(double progress) const override { return t0 * (1.0 - progress * progress); }

  std::string toString() const override
  {
    std::stringstream ss;
    ss << "Quadratic";
    if (t0 != 1) {
      ss << "[t0=" << t0 << "]";
    }
    return ss.str();
  }

private:
  double t0;
};

class Cosine final : public ICooling
{
public:
  Cosine(double temperature_ = 1.0) : t0(temperature_) {}

  double getTemperature(double progress) const override { return t0 * (1.0 + std::cos(M_PI * progress)) / 2.0; }

  std::string toString() const override
  {
    std::stringstream ss;
    ss << "Cosine";
    if (t0 != 1) {
      ss << "[t0=" << t0 << "]";
    }
    return ss.str();
  }

private:
  double t0;
};

}  // namespace sa::policies

#endif  // SIMULATED_ANNEALING_POLICIES_COOLING_H_