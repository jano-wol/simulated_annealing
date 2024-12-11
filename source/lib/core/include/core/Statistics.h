#ifndef SIMULATED_ANNEALING_CORE_STATISTICS_H_
#define SIMULATED_ANNEALING_CORE_STATISTICS_H_

#include <cmath>
#include <numeric>
#include <optional>

#include <core/CircularBuffer.h>

namespace sa::core
{
class Statistics
{
public:
  Statistics(std::span<const double> data);

  int size() const;

  std::optional<double> mean;
  std::optional<double> deviation;
};
}  // namespace sa::core

#endif  // SIMULATED_ANNEALING_CORE_STATISTICS_H_