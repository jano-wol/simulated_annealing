#include <policies/Cooling.h>

#include <cmath>
#include <sstream>

using namespace sa::policies;

double Linear::getTemperature(double progress) const { return t0 * (1.0 - progress); }

std::string Linear::toString() const
{
  std::stringstream ss;
  ss << "Linear";
  if (t0 != 1) {
    ss << "[t0=" << t0 << "]";
  }
  return ss.str();
}

double Exponential::getTemperature(double progress) const { return t0 * std::exp(c * progress); }

std::string Exponential::toString() const
{
  std::stringstream ss;
  ss << "Exponential[c=" << c;
  if (t0 != 1) {
    ss << ";t0=" << t0;
  }
  ss << "]";
  return ss.str();
}

double Logarithmic::getTemperature(double progress) const { return t0 / (1 + c * std::log(1.0 + progress)); }

std::string Logarithmic::toString() const
{
  std::stringstream ss;
  ss << "Logarithmic[c=" << c;
  if (t0 != 1) {
    ss << ";t0=" << t0;
  }
  ss << "]";
  return ss.str();
}

double Quadratic::getTemperature(double progress) const { return t0 * (1.0 - progress * progress); }

std::string Quadratic::toString() const
{
  std::stringstream ss;
  ss << "Quadratic";
  if (t0 != 1) {
    ss << "[t0=" << t0 << "]";
  }
  return ss.str();
}

double Cosine::getTemperature(double progress) const { return t0 * (1.0 + std::cos(M_PI * progress)) / 2.0; }

std::string Cosine::toString() const
{
  std::stringstream ss;
  ss << "Cosine";
  if (t0 != 1) {
    ss << "[t0=" << t0 << "]";
  }
  return ss.str();
}
