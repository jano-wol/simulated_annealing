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
  Statistics(std::span<const double> data)
  {
    if (data.empty()) {
      mean = std::nullopt;
      deviation = std::nullopt;
      return;
    }
    double sum = std::accumulate(data.begin(), data.end(), 0.0);
    mean = sum / data.size();

    double variance = 0.0;
    for (double value : data) {
      variance += (value - *mean) * (value - *mean);
    }
    variance /= data.size();
    deviation = std::sqrt(variance);
  };

  std::optional<double> mean;
  std::optional<double> deviation;
};
}  // namespace sa::core

#endif  // SIMULATED_ANNEALING_CORE_STATISTICS_H_