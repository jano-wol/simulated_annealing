#include <salesman_angle/Position.h>

#include <cmath>
#include <iomanip>
#include <sstream>

#include <imgui/imgui.h>
#include <implot/implot.h>

#include <core/IPosition.h>
#include <core/Random.h>
#include <core/Rounding.h>
#include <salesman_angle/Move.h>

using namespace sa::core;
using namespace sa::targets::salesman_angle;

double SalesmanAnglePosition::getEnergy() const { return energy; }

IMove::CPtr SalesmanAnglePosition::generateMove() const
{
  if (cities.size() < 2) {
    return std::make_unique<SalesmanAngleMove>(0, 0, 0);
  }
  std::size_t idx1 = r.randomInt(0, cities.size() - 1);
  std::size_t idx2 = r.randomInt(0, cities.size() - 2);
  if (idx2 >= idx1) {
    ++idx2;
  } else {
    std::swap(idx1, idx2);
  }
  if (idx1 == 0 && idx2 == cities.size() - 1) {
    return std::make_unique<SalesmanAngleMove>(idx1, idx2, 0);
  }
  const auto& [prevIdx1, nextIdx1] = getNeighbourIdxs(idx1);
  const auto& [prevIdx2, nextIdx2] = getNeighbourIdxs(idx2);
  double delta = distance(cities[idx1], cities[nextIdx2]) + distance(cities[idx2], cities[prevIdx1]) -
                 distance(cities[idx1], cities[prevIdx1]) - distance(cities[idx2], cities[nextIdx2]);
  return std::make_unique<SalesmanAngleMove>(idx1, idx2, delta);
}

void SalesmanAnglePosition::makeMove(IMove::CPtr move)
{
  auto* m = dynamic_cast<SalesmanAngleMove*>(move.get());
  energy += m->getDelta();
  std::size_t cityIdx1 = m->cityIdx1;
  std::size_t cityIdx2 = m->cityIdx2;
  if (cityIdx1 == cityIdx2) {
    return;
  }
  if (cityIdx2 < cityIdx1) {
    std::swap(cityIdx1, cityIdx2);
  }
  while (cityIdx1 < cityIdx2) {
    std::swap(cities[cityIdx1], cities[cityIdx2]);
    ++cityIdx1;
    --cityIdx2;
  }
}

int SalesmanAnglePosition::size() const
{
  return sizeof(double) + sizeof(std::vector<std::pair<double, double>>) +
         sizeof(std::pair<double, double>) * cities.capacity();
}

IPosition::CPtr SalesmanAnglePosition::clone() const { return std::make_unique<SalesmanAnglePosition>(energy, cities); }

std::pair<std::size_t, std::size_t> SalesmanAnglePosition::getNeighbourIdxs(std::size_t idx) const
{
  std::size_t prevIdx = (idx == 0) ? (cities.size() - 1) : idx - 1;
  std::size_t nextIdx = (idx == (cities.size() - 1)) ? 0 : idx + 1;
  return {prevIdx, nextIdx};
}

double SalesmanAnglePosition::distance(const std::pair<double, double>& city1,
                                       const std::pair<double, double>& city2) const
{
  const auto& [x1, y1] = city1;
  const auto& [x2, y2] = city2;
  double ret = ((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
  return std::sqrt(ret);
}

double SalesmanAnglePosition::calcEnergy() const
{
  if (cities.empty() || cities.size() == 1) {
    return 0;
  }
  double ret = 0;
  for (std::size_t idx = 0; idx < cities.size(); ++idx) {
    const auto& [prevIdx, nextIdx] = getNeighbourIdxs(idx);
    ret += distance(cities[idx], cities[nextIdx]);
  };
  return ret;
}

std::string SalesmanAnglePosition::toString(const IPosition::CPtr& iPosition)
{
  SalesmanAnglePosition* position = dynamic_cast<SalesmanAnglePosition*>(iPosition.get());
  std::stringstream ss;
  ss << std::setprecision(Rounding::precision) << std::fixed;
  ss << getTypeId() << " ";
  for (const auto& [d1, d2] : position->cities) {
    ss << d1 << " " << d2 << " ";
  }
  return ss.str();
}

IPosition::CPtr SalesmanAnglePosition::fromString(const std::string& data)
{
  std::istringstream ss(data);
  std::string typeId;
  ss >> typeId;

  std::vector<std::pair<double, double>> cities;
  double d1, d2;
  while (ss >> d1 >> d2) {
    cities.emplace_back(Rounding::roundDouble(d1), Rounding::roundDouble(d2));
  }
  auto position = std::make_unique<SalesmanAnglePosition>(std::move(cities));
  return position;
}

std::string SalesmanAnglePosition::getTypeId() { return "salesman_angle"; }

void SalesmanAnglePosition::plot() const
{
  for (const auto& [x, y] : cities) {
    ImPlot::PlotScatter("Points", &x, &y, 1);
  }
  for (size_t idx = 0; idx < cities.size(); ++idx) {
    const auto& [x1, y1] = cities[idx];
    const auto& [x2, y2] = cities[(idx + 1) % cities.size()];
    double xs[2] = {x1, x2};
    double ys[2] = {y1, y2};
    ImPlot::PlotLine("Edges", xs, ys, 2);
  }
}
