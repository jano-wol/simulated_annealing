#ifndef SIMULATED_ANNEALING_TARGETS_MINIMAL_SPANNING_TREE_POSITION_H_
#define SIMULATED_ANNEALING_TARGETS_MINIMAL_SPANNING_TREE_POSITION_H_

#include <unordered_set>
#include <vector>

#include <core/IPosition.h>
#include <core/Random.h>
#include <minimal_spanning_tree/Move.h>
#include <serializator/Serializator.h>

namespace sa::targets::minimal_spanning_tree
{
class MinimalSpanningTreePosition final : public core::IPosition
{
public:
  MinimalSpanningTreePosition(std::vector<std::pair<double, double>> cities_) : cities(std::move(cities_))
  {
    energy = 0;
    for (int i = 1; i < int(cities.size()); ++i) {
      adj[0].insert(i);
      adj[i].insert(0);
      edgeList.push_back({0, i});
      energy += distance(0, i);
    }
  }

  MinimalSpanningTreePosition(double energy_, std::vector<std::pair<double, double>> cities_,
                              std::vector<std::unordered_set<int>> adj_, std::vector<std::pair<int, int>> edgeList_)
      : energy(energy_), cities(std::move(cities_)), adj(std::move(adj_)), edgeList(std::move(edgeList_))
  {}

  static std::string toString(const core::IPosition::CPtr& position);
  static core::IPosition::CPtr fromString(const std::string& data);

  double getEnergy() const override;
  core::IMove::CPtr generateMoveCandidate() const override;
  void makeMove(core::IMove::CPtr move) override;
  core::IPosition::CPtr clone() const override;
  int size() const override;
  void plot() const override;

  double distance(std::size_t u, std::size_t v) const;
  std::vector<int> getPerturbateNeighborhood(int u) const;
  int getNeighbor(const std::vector<int>& neighbors, int forbidden) const;

  double energy;
  std::vector<std::pair<double, double>> cities;
  std::vector<std::unordered_set<int>> adj;
  std::vector<std::pair<int, int>> edgeList;
  core::Random r;

  REGISTER_TARGET_BY_ID(minimal_spanning_tree)
};
}  // namespace sa::targets::minimal_spanning_tree

#endif  // SIMULATED_ANNEALING_TARGETS_MINIMAL_SPANNING_TREE_POSITION_H_
