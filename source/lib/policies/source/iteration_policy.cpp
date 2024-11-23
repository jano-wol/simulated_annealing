#include <policies/iteration_policy.h>

using namespace sa::policies;

double IterationPolicy::getResourceAll() const { return allIterations; }

double IterationPolicy::getResourceLeft() const { return leftIterations; }

void IterationPolicy::updateResourceLeft()
{
  if (leftIterations > 0) {
    --leftIterations;
  }
}