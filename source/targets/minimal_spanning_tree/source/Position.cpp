#include <minimal_spanning_tree/Position.h>

#include <cmath>
#include <iomanip>
#include <numbers>
#include <sstream>

#include <imgui/imgui.h>
#include <implot/implot.h>

#include <core/IPosition.h>
#include <core/Random.h>
#include <core/Rounding.h>
#include <minimal_spanning_tree/Move.h>

using namespace sa::core;
using namespace sa::targets::minimal_spanning_tree;

int MinimalSpanningTreePosition::getNeighbor(const std::vector<int>& neighbors, int forbidden) const
{
  int idx = r.randomInt(0, neighbors.size() - 1);
  if (neighbors[idx] == forbidden) {
    int idx2 = r.randomInt(0, neighbors.size() - 2);
    if (idx2 >= idx) {
      ++idx2;
    }
    idx = idx2;
  }
  return neighbors[idx];
}

double MinimalSpanningTreePosition::getEnergy() const { return energy; }

IMove::CPtr MinimalSpanningTreePosition::generateMoveCandidate() const
{
  if (cities.size() < 2) {
    return std::make_unique<MinimalSpanningTreeMove>(0, 0, 0, 0.0);
  }
  int deleteEdgeIdx = r.randomInt(0, edgeList.size() - 1);
  const auto& [u, v] = edgeList[deleteEdgeIdx];
  auto neighborU = getPerturbateNeighborhood(u);
  auto neighborV = getPerturbateNeighborhood(v);
  int uu = getNeighbor(neighborU, v);
  int vv = getNeighbor(neighborV, u);
  double delta = distance(uu, vv);
  delta -= distance(u, v);
  return std::make_unique<MinimalSpanningTreeMove>(deleteEdgeIdx, std::min(uu, vv), std::max(uu, vv), delta);
}

void MinimalSpanningTreePosition::makeMove(IMove::CPtr move)
{
  auto* m = dynamic_cast<MinimalSpanningTreeMove*>(move.get());
  int deleteEdgeIdx = m->deleteEdgeIdx;
  int u = m->addEdgeU;
  int v = m->addEdgeV;
  const auto& [uu, vv] = edgeList[deleteEdgeIdx];
  if (u == uu && v == vv) {
    return;
  }
  energy += m->getDelta();
  edgeList[deleteEdgeIdx] = {u, v};
  adj[u].insert(v);
  adj[v].insert(u);
  adj[uu].erase(vv);
  adj[vv].erase(uu);
}

int MinimalSpanningTreePosition::size() const
{
  int totalSize = sizeof(MinimalSpanningTreePosition);
  totalSize += cities.capacity() * sizeof(std::pair<double, double>);
  totalSize += edgeList.capacity() * sizeof(std::pair<int, int>);
  totalSize += adj.capacity() * sizeof(std::unordered_set<int>);
  for (const auto& set : adj) {
    totalSize += set.size() * sizeof(int);
    totalSize += set.bucket_count() * sizeof(void*);
  }
  return totalSize;
}

IPosition::CPtr MinimalSpanningTreePosition::clone() const
{
  return std::make_unique<MinimalSpanningTreePosition>(energy, cities, adj, edgeList);
}

double MinimalSpanningTreePosition::distance(std::size_t u, std::size_t v) const
{
  const auto& [x1, y1] = cities[u];
  const auto& [x2, y2] = cities[v];
  double ret = ((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
  return std::sqrt(ret);
}

std::vector<int> MinimalSpanningTreePosition::getPerturbateNeighborhood(int u) const
{
  std::vector<int> neighbors(adj[u].begin(), adj[u].end());
  neighbors.push_back(u);
  return neighbors;
}

std::string MinimalSpanningTreePosition::toString(const IPosition::CPtr& iPosition)
{
  MinimalSpanningTreePosition* position = dynamic_cast<MinimalSpanningTreePosition*>(iPosition.get());
  std::stringstream ss;
  ss << std::setprecision(Rounding::precision) << std::fixed;
  ss << position->cities.size() << " ";
  for (const auto& [d1, d2] : position->cities) {
    ss << d1 << " " << d2 << " ";
  }
  for (const auto& [d1, d2] : position->edgeList) {
    ss << d1 << " " << d2 << " ";
  }
  return ss.str();
}

IPosition::CPtr MinimalSpanningTreePosition::fromString(const std::string& data)
{
  std::istringstream ss(data);
  std::vector<std::pair<double, double>> cities;
  std::vector<std::pair<int, int>> edgeList;
  std::size_t n;
  ss >> n;
  for (std::size_t i = 0; i < n; ++i) {
    double d1, d2;
    ss >> d1 >> d2;
    cities.emplace_back(Rounding::roundDouble(d1), Rounding::roundDouble(d2));
  }
  double energy = 0;
  std::vector<std::unordered_set<int>> adj(n);
  for (std::size_t i = 0; i < n; ++i) {
    int d1, d2;
    ss >> d1 >> d2;
    edgeList.push_back({std::min(d1, d2), std::max(d1, d2)});
    adj[d1].insert(d2);
    adj[d2].insert(d1);
    const auto& [x1, y1] = cities[d1];
    const auto& [x2, y2] = cities[d2];
    double ret = ((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
    energy += std::sqrt(ret);
  }
  auto position =
      std::make_unique<MinimalSpanningTreePosition>(energy, std::move(cities), std::move(adj), std::move(edgeList));
  return position;
}

void MinimalSpanningTreePosition::plot() const
{
  for (const auto& [x, y] : cities) {
    ImPlot::PlotScatter("Points", &x, &y, 1);
  }
  for (auto [u, v] : edgeList) {
    const auto& [x1, y1] = cities[u];
    const auto& [x2, y2] = cities[v];
    double xs[2] = {x1, x2};
    double ys[2] = {y1, y2};
    ImPlot::PlotLine("Edges", xs, ys, 2);
  }
}
