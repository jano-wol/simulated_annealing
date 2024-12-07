#ifndef SIMULATED_ANNEALING_CORE_STATISTICS_H_
#define SIMULATED_ANNEALING_CORE_STATISTICS_H_

namespace sa::core
{
class Statistics
{
public:
  Statistics(){};

  double min;
  double max;
  double mean;
};
}  // namespace sa::core

#endif  // SIMULATED_ANNEALING_CORE_STATISTICS_H_