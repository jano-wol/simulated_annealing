#include <core/Statistics.h>

using namespace sa::core;

Statistics::Statistics(std::span<const double> data)
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

int Statistics::size() const { return sizeof(*this); }
