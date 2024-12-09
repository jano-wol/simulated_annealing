#ifndef SIMULATED_ANNEALING_CORE_CIRCULAR_BUFFER_H
#define SIMULATED_ANNEALING_CORE_CIRCULAR_BUFFER_H

#include <span>
#include <vector>

namespace sa::core
{
class CircularBuffer
{
public:
  explicit CircularBuffer(std::size_t size) : buffer(size), head(0), count(0) {}

  void push(double value);
  std::span<const double> getData() const;
  bool isEmpty() const;
  double getNewest() const;
  double getOldest() const;
  std::size_t getCount() const;

private:
  std::vector<double> buffer;
  std::size_t head;
  std::size_t count;
};
}  // namespace sa::core

#endif  // SIMULATED_ANNEALING_CORE_CIRCULAR_BUFFER_H