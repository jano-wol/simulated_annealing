#ifndef SIMULATED_ANNEALING_POLICIES_RESOURCE_H_
#define SIMULATED_ANNEALING_POLICIES_RESOURCE_H_

namespace sa::policies
{
class Iteration
{
public:
  Iteration(double all_) : all(all_), left(all_) {}
  double getAll() const { return all; }
  double getLeft() const { return left; }
  void updateLeft()
  {
    if (left > 0) {
      --left;
    }
  }

private:
  double all;
  double left;
};
}  // namespace sa::policies

#endif  // SIMULATED_ANNEALING_POLICIES_RESOURCE_H_