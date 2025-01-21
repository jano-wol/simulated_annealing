#ifndef SIMULATED_ANNEALING_TARGETS_MINIMAL_SPANNING_TREE_MOVE_H_
#define SIMULATED_ANNEALING_TARGETS_MINIMAL_SPANNING_TREE_MOVE_H_

#include <cstdlib>

#include <core/IMove.h>

namespace sa::targets::minimal_spanning_tree
{
class MinimalSpanningTreeMove final : public core::IMove
{
public:
  MinimalSpanningTreeMove(std::size_t addIdx1_, std::size_t addIdx2_, std::size_t cutIdx1_, std::size_t cutIdx2_,
                          double delta_)
      : addIdx1(addIdx1_), addIdx2(addIdx2_), cutIdx1(cutIdx1_), cutIdx2(cutIdx2_), delta(delta_)
  {}

  double getDelta() const override;
  int size() const override;

  std::size_t addIdx1;
  std::size_t addIdx2;
  std::size_t cutIdx1;
  std::size_t cutIdx2;
  double delta;
};
}  // namespace sa::targets::minimal_spanning_tree

#endif  // SIMULATED_ANNEALING_TARGETS_MINIMAL_SPANNING_TREE_MOVE_H_
