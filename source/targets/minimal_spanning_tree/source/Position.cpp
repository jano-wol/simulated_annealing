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

double MinimalSpanningTreePosition::getEnergy() const { return energy; }

IMove::CPtr MinimalSpanningTreePosition::generateMove() const
{
  if (cities.size() < 2) {
    return std::make_unique<MinimalSpanningTreeMove>(0, 0, 0, 0, 0);
  }
  std::size_t idx1 = r.randomInt(0, int(cities.size()) - 1);
  std::size_t idx2 = r.randomInt(0, int(cities.size()) - 2);
  if (idx2 >= idx1) {
    ++idx2;
  } else {
    std::swap(idx1, idx2);
  }
  if (tree->isTreeEdge(idx1, idx2)) {
    return std::make_unique<MinimalSpanningTreeMove>(0, 0, 0, 0, 0);
  }
  std::size_t addIdx1 = idx1;
  std::size_t addIdx2 = idx2;
  auto path = tree->getPath(idx1, idx2);
  std::size_t idx = r.randomInt(0, int(path.size()) - 1);
  std::size_t cutIdx1 = path[idx];
  std::size_t cutIdx2 = path[idx + 1];
  double delta = distance(addIdx1, addIdx2);
  delta -= distance(cutIdx1, cutIdx2);
  return std::make_unique<MinimalSpanningTreeMove>(addIdx1, addIdx2, cutIdx1, cutIdx2, delta);
}

void MinimalSpanningTreePosition::makeMove(IMove::CPtr move)
{
  auto* m = dynamic_cast<MinimalSpanningTreeMove*>(move.get());
  energy += m->getDelta();
  std::size_t addIdx1 = m->addIdx1;
  std::size_t addIdx2 = m->addIdx2;
  std::size_t cutIdx1 = m->cutIdx1;
  std::size_t cutIdx2 = m->cutIdx2;
  tree->cut(cutIdx1, cutIdx2);
  tree->link(addIdx1, addIdx2);
}

int MinimalSpanningTreePosition::size() const
{
  return int(sizeof(double) + sizeof(std::vector<std::pair<double, double>>) +
             sizeof(std::pair<double, double>) * cities.capacity()) +
         tree->size();
}

IPosition::CPtr MinimalSpanningTreePosition::clone() const
{
  return std::make_unique<MinimalSpanningTreePosition>(energy, cities, tree->clone());
}

double MinimalSpanningTreePosition::distance(std::size_t u, std::size_t v) const
{
  const auto& [x1, y1] = cities[u];
  const auto& [x2, y2] = cities[v];
  double ret = ((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
  return std::sqrt(ret);
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
  auto edges = position->tree->getEdges();
  for (const auto& [d1, d2] : edges) {
    ss << d1 << " " << d2 << " ";
  }
  return ss.str();
}

IPosition::CPtr MinimalSpanningTreePosition::fromString(const std::string& data)
{
  std::istringstream ss(data);
  std::vector<std::pair<double, double>> cities;
  std::size_t n;
  ss >> n;
  for (std::size_t i = 0; i < n; ++i) {
    double d1, d2;
    ss >> d1 >> d2;
    cities.emplace_back(Rounding::roundDouble(d1), Rounding::roundDouble(d2));
  }
  auto tree = std::make_unique<LinkCutTree>(int(n));
  double energy = 0;
  for (std::size_t i = 0; i < n; ++i) {
    int d1, d2;
    ss >> d1 >> d2;
    tree->link(d1, d2);
    const auto& [x1, y1] = cities[d1];
    const auto& [x2, y2] = cities[d2];
    double ret = ((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
    energy += std::sqrt(ret);
  }
  auto position = std::make_unique<MinimalSpanningTreePosition>(energy, std::move(cities), std::move(tree));
  return position;
}

void MinimalSpanningTreePosition::plot() const
{
  for (const auto& [x, y] : cities) {
    ImPlot::PlotScatter("Points", &x, &y, 1);
  }
  auto edges = tree->getEdges();
  for (auto [u, v] : edges) {
    const auto& [x1, y1] = cities[u];
    const auto& [x2, y2] = cities[v];
    double xs[2] = {x1, x2};
    double ys[2] = {y1, y2};
    ImPlot::PlotLine("Edges", xs, ys, 2);
  }
}
