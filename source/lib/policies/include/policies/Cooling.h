#ifndef SIMULATED_ANNEALING_POLICIES_COOLING_H_
#define SIMULATED_ANNEALING_POLICIES_COOLING_H_

#include <algorithm>
#include <cmath>
#include <sstream>
#include <string>

namespace sa::policies
{
class Linear
{
public:
  Linear(double temperature_ = 1.0) : t0(temperature_) {}

  double getTemperature(double progress) const { return t0 * (1.0 - progress); }

  std::string toString() const
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

class Exponential
{
public:
  Exponential(double c_, double temperature_ = 1.0) : c(c_), t0(temperature_) {}

  double getTemperature(double progress) const { return t0 * std::exp(c * progress); }

  std::string toString() const
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

class Logarithmic
{
public:
  Logarithmic(double c_, double temperature_ = 1.0) : c(c_), t0(temperature_) {}

  double getTemperature(double progress) const { return t0 / (1 + c * std::log(1.0 + progress)); }

  std::string toString() const
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

class Quadratic
{
public:
  Quadratic(double temperature_ = 1.0) : t0(temperature_) {}

  double getTemperature(double progress) const { return t0 * (1.0 - progress * progress); }

  std::string toString() const
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

class Cosine
{
public:
  Cosine(double temperature_ = 1.0) : t0(temperature_) {}

  double getTemperature(double progress) const { return t0 * (1.0 + std::cos(M_PI * progress)) / 2.0; }

  std::string toString() const
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