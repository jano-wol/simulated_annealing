#include <cstddef>

#include <policies/IResourcePolicy.h>

namespace sa::policies
{
class IterationPolicy : public IResourcePolicy
{
public:
  IterationPolicy(double allIterations_) : allIterations(allIterations_), leftIterations(allIterations_) {}
  double getResourceAll() const override;
  double getResourceLeft() const override;
  void updateResourceLeft();

private:
  double allIterations;
  double leftIterations;
};
}  // namespace sa::policies