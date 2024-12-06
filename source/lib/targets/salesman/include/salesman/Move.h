#ifndef SIMULATED_ANNEALING_LIB_TARGETS_SALESMAN_SALESMANMOVE_H_
#define SIMULATED_ANNEALING_LIB_TARGETS_SALESMAN_SALESMANMOVE_H_

#include <cstdlib>

#include <core/IMove.h>

namespace sa::targets::salesman
{
class SalesmanMove : public core::IMove
{
public:
  SalesmanMove(std::size_t cityIdx1_, std::size_t cityIdx2_) : cityIdx1(cityIdx1_), cityIdx2(cityIdx2_) {}

  int size() const override { return 2 * sizeof(std::size_t); }

  std::size_t cityIdx1;
  std::size_t cityIdx2;
};
}  // namespace sa::targets::salesman

#endif  // SIMULATED_ANNEALING_LIB_TARGETS_SALESMAN_SALESMANMOVE_H_
