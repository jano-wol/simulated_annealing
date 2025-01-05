#ifndef SIMULATED_ANNEALING_TARGETS_SALESMAN_ANGLE_MOVE_H_
#define SIMULATED_ANNEALING_TARGETS_SALESMAN_ANGLE_MOVE_H_

#include <cstdlib>

#include <core/IMove.h>

namespace sa::targets::salesman_angle
{
class SalesmanAngleMove final : public core::IMove
{
public:
  SalesmanAngleMove(std::size_t cityIdx1_, std::size_t cityIdx2_, double delta_)
      : cityIdx1(cityIdx1_), cityIdx2(cityIdx2_), delta(delta_)
  {}

  double getDelta() const override;
  int size() const override;

  std::size_t cityIdx1;
  std::size_t cityIdx2;
  double delta;
};
}  // namespace sa::targets::salesman_angle

#endif  // SIMULATED_ANNEALING_TARGETS_SALESMAN_ANGLE_MOVE_H_
