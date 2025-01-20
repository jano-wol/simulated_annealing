#ifndef SIMULATED_ANNEALING_TARGETS_MINIMAL_SPANNING_TREE_POSITION_H_
#define SIMULATED_ANNEALING_TARGETS_MINIMAL_SPANNING_TREE_POSITION_H_

#include <vector>

#include <core/IPosition.h>
#include <core/Random.h>
#include <minimal_spanning_tree/LinkCutTree.h>
#include <minimal_spanning_tree/Move.h>
#include <serializator/Serializator.h>

namespace sa::targets::minimal_spanning_tree
{
class MinimalSpanningTreePosition final : public core::IPosition
{
public:
  MinimalSpanningTreePosition(std::vector<std::pair<double, double>> cities_)
      : cities(std::move(cities_)), tree(std::make_unique<LinkCutTree>(cities.size()))
  {
    energy = 0;
    for (int i = 1; i < int(cities.size()); ++i) {
      tree->link(0, i);
      energy += distance(0, i);
    }
  }

  MinimalSpanningTreePosition(double energy_, std::vector<std::pair<double, double>> cities_, LinkCutTree::CPtr tree_)
      : energy(energy_), cities(std::move(cities_)), tree(std::move(tree_))
  {}

  static std::string toString(const core::IPosition::CPtr& position);
  static core::IPosition::CPtr fromString(const std::string& data);

  double getEnergy() const override;
  core::IMove::CPtr generateMove() const override;
  void makeMove(core::IMove::CPtr move) override;
  int size() const override;
  core::IPosition::CPtr clone() const override;
  void plot() const override;

  double distance(std::size_t u, std::size_t v) const;

  double energy;
  std::vector<std::pair<double, double>> cities;
  LinkCutTree::CPtr tree;
  core::Random r;

  REGISTER_TARGET_BY_ID(minimal_spanning_tree)
};
}  // namespace sa::targets::minimal_spanning_tree

#endif  // SIMULATED_ANNEALING_TARGETS_MINIMAL_SPANNING_TREE_POSITION_H_
