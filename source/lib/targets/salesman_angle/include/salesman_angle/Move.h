#ifndef SIMULATED_ANNEALING_TARGETS_SALESMAN_ANGLE_MOVE_H_
#define SIMULATED_ANNEALING_TARGETS_SALESMAN_ANGLE_MOVE_H_

#include <cstdlib>

#include <core/IMove.h>

namespace sa::targets::salesman_angle
{
class SalesmanAngleMove final : public core::IMove
{
public:
  SalesmanAngleMove(std::size_t idx_, std::size_t shiftIdx_, double delta_)
      : idx(idx_), shiftIdx(shiftIdx_), delta(delta_)
  {}

  double getDelta() const override;
  int size() const override;

  std::size_t idx;
  std::size_t shiftIdx;
  double delta;
};
}  // namespace sa::targets::salesman_angle

#endif  // SIMULATED_ANNEALING_TARGETS_SALESMAN_ANGLE_MOVE_H_
