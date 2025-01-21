#ifndef SIMULATED_ANNEALING_TARGETS_MINIMAL_SPANNING_TREE_GENERATOR_H_
#define SIMULATED_ANNEALING_TARGETS_MINIMAL_SPANNING_TREE_GENERATOR_H_

#include <core/IGenerator.h>

namespace sa::targets::minimal_spanning_tree
{
class MinimalSpanningTreeGenerator final : public core::IGenerator
{
public:
  MinimalSpanningTreeGenerator(int numCities_, int length_, bool subSquares_)
      : numCities(numCities_), length(length_), subSquares(subSquares_)
  {}

  core::IPosition::CPtr generatePosition(int idx) const override;
  std::string getGeneratorName() const override;

private:
  core::IPosition::CPtr getStartPosition(int idx) const;
  core::IPosition::CPtr getStartPosition_5x5(int idx) const;

  int numCities;
  int length;
  bool subSquares;
};
}  // namespace sa::targets::minimal_spanning_tree

#endif  // SIMULATED_ANNEALING_TARGETS_MINIMAL_SPANNING_TREE_GENERATOR_H_
