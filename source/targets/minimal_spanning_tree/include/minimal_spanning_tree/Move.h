#ifndef SIMULATED_ANNEALING_TARGETS_MINIMAL_SPANNING_TREE_MOVE_H_
#define SIMULATED_ANNEALING_TARGETS_MINIMAL_SPANNING_TREE_MOVE_H_

#include <cstdlib>

#include <core/IMove.h>

namespace sa::targets::minimal_spanning_tree
{
class MinimalSpanningTreeMove final : public core::IMove
{
public:
  MinimalSpanningTreeMove(int deleteIdx_, int u_, int v_, double delta_)
      : deleteIdx(deleteIdx_), u(u_), v(v_), delta(delta_)
  {}

  double getDelta() const override;
  int size() const override;

  int deleteIdx;
  int u;
  int v;
  double delta;
};
}  // namespace sa::targets::minimal_spanning_tree

#endif  // SIMULATED_ANNEALING_TARGETS_MINIMAL_SPANNING_TREE_MOVE_H_
