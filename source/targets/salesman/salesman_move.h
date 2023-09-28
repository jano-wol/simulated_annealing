#ifndef SIMULATED_ANNEALING_TARGETS_SALESMAN_SALESMANMOVE_H_
#define SIMULATED_ANNEALING_TARGETS_SALESMAN_SALESMANMOVE_H_

#include <core/IMove.h>
#include <cstdlib>

using namespace sa::core;

class SalesmanMove : public IMove
{
public:
  SalesmanMove(std::size_t cityIdx1_, std::size_t cityIdx2_, bool inner_)
      : cityIdx1(cityIdx1_), cityIdx2(cityIdx2_), inner(inner_)
  {}
  std::size_t cityIdx1;
  std::size_t cityIdx2;
  bool inner;
};

#endif  // SIMULATED_ANNEALING_TARGETS_SALESMAN_SALESMANMOVE_H_
