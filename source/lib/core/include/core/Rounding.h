#ifndef SIMULATED_ANNEALING_CORE_ROUNDING_H_
#define SIMULATED_ANNEALING_CORE_ROUNDING_H_

#include <cmath>

namespace sa::core
{
class Rounding
{
public:
  static double roundDouble(double value)
  {
    double scale = std::pow(10.0, precision);
    return std::round(value * scale) / scale;
  }

  static const int precision = 4;
};
}  // namespace sa::core

#endif  // SIMULATED_ANNEALING_CORE_ROUNDING_H_
