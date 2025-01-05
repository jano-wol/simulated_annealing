#ifndef SIMULATED_ANNEALING_TARGETS_SALESMAN_AREA_MAX_MOVE_H_
#define SIMULATED_ANNEALING_TARGETS_SALESMAN_AREA_MAX_MOVE_H_

#include <cstdlib>

#include <core/IMove.h>

namespace sa::targets::salesman_area_max
{
class SalesmanAreaMaxMove final : public core::IMove
{
public:
  SalesmanAreaMaxMove(std::size_t idx_, std::size_t shiftIdx_, double delta_, double signedDelta_)
      : idx(idx_), shiftIdx(shiftIdx_), delta(delta_), signedDelta(signedDelta_)
  {}

  double getDelta() const override;
  int size() const override;

  std::size_t idx;
  std::size_t shiftIdx;
  double delta;
  double signedDelta;
};
}  // namespace sa::targets::salesman_area_max

#endif  // SIMULATED_ANNEALING_TARGETS_SALESMAN_AREA_MAX_MOVE_H_
