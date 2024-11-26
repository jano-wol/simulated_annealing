#ifndef SIMULATED_ANNEALING_TARGETS_SALESMAN_SALESMANMOVE_H_
#define SIMULATED_ANNEALING_TARGETS_SALESMAN_SALESMANMOVE_H_

#include <cstdlib>

#include <core/IMove.h>

class SalesmanMove : public sa::core::IMove
{
public:
  SalesmanMove(std::size_t idx1_, std::size_t idx2_) : idx1(idx1_), idx2(idx2_) {}
  std::size_t idx1;
  std::size_t idx2;
};

#endif  // SIMULATED_ANNEALING_TARGETS_SALESMAN_SALESMANMOVE_H_
