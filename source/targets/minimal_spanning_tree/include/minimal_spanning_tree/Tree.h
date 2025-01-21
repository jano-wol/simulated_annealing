#ifndef SIMULATED_ANNEALING_TARGETS_MINIMAL_SPANNING_TREE_LINK_CUT_TREE_H_
#define SIMULATED_ANNEALING_TARGETS_MINIMAL_SPANNING_TREE_LINK_CUT_TREE_H_

#include <algorithm>
#include <memory>
#include <set>
#include <vector>

namespace sa::targets::minimal_spanning_tree
{
class Tree
{
public:
  using CPtr = std::unique_ptr<Tree>;

  Tree(int n) : inc(n) {}

  void link(int u, int v)
  {
    inc[u].insert(v);
    inc[v].insert(u);

    if (u < v) {
      edges.insert({u, v});
    } else {
      edges.insert({v, u});
    }
  }

  void cut(int u, int v)
  {
    inc[u].erase(v);
    inc[v].erase(u);

    if (u < v) {
      edges.erase({u, v});
    } else {
      edges.erase({v, u});
    }
  }

  std::vector<int> getPath(int u, int v)
  {
    std::vector<int> distances(inc.size(), -1);
    distances[v] = 0;
    std::set<int> active;
    active.insert(v);
    int currDist = 1;
    while (distances[u] == -1) {
      std::set<int> curr;
      for (auto w : active) {
        for (auto n : inc[w]) {
          if (distances[n] == -1) {
            distances[n] = currDist;
            curr.insert(n);
          }
        }
      }
      active = curr;
      ++currDist;
    }

    std::vector<int> ret;
    int currVertex = u;
    currDist = distances[currVertex];
    while (currVertex != v) {
      ret.push_back(currVertex);
      for (auto n : inc[currVertex]) {
        if (distances[n] == currDist - 1) {
          currVertex = n;
          currDist = currDist - 1;
          break;
        }
      }
    }
    ret.push_back(v);
    return ret;
  }

  bool isTreeEdge(int u, int v) { return (inc[u].find(v) != inc[u].end()); }

  const std::set<std::pair<int, int>>& getEdges() { return edges; }

  Tree::CPtr clone()
  {
    Tree::CPtr clonedTree = std::make_unique<Tree>(int(inc.size()));
    clonedTree->inc = inc;
    clonedTree->edges = edges;
    return clonedTree;
  }

  int size()
  {
    int ret = sizeof(*this);
    for (const auto& s : inc) {
      ret += int(sizeof(int) * s.size());
    }
    ret += int(sizeof(std::pair<int, int>) * edges.size());
    return ret;
  }

  std::vector<std::set<int>> inc;
  std::set<std::pair<int, int>> edges;
};
}  // namespace sa::targets::minimal_spanning_tree

#endif  // SIMULATED_ANNEALING_TARGETS_MINIMAL_SPANNING_TREE_LINK_CUT_TREE_H_
