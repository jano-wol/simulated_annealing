#ifndef SIMULATED_ANNEALING_TARGETS_MINIMAL_SPANNING_TREE_MOVE_H_
#define SIMULATED_ANNEALING_TARGETS_MINIMAL_SPANNING_TREE_MOVE_H_

#include <cstdlib>

#include <core/IMove.h>

namespace sa::targets::minimal_spanning_tree
{
class MinimalSpanningTreeMove final : public core::IMove
{
public:
  MinimalSpanningTreeMove(int deleteEdgeIdx_, int addEdgeU_, int addEdgeV_, double delta_)
      : deleteEdgeIdx(deleteEdgeIdx_), addEdgeU(addEdgeU_), addEdgeV(addEdgeV_), delta(delta_)
  {}

  double getDelta() const override;
  int size() const override;

  int deleteEdgeIdx;
  int addEdgeU;
  int addEdgeV;
  double delta;
};
}  // namespace sa::targets::minimal_spanning_tree

#endif  // SIMULATED_ANNEALING_TARGETS_MINIMAL_SPANNING_TREE_MOVE_H_
