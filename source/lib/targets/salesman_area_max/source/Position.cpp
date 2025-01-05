#include <salesman_area_max/Position.h>

#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>

#include <imgui/imgui.h>
#include <implot/implot.h>

#include <core/IPosition.h>
#include <core/Random.h>
#include <core/Rounding.h>
#include <salesman_area_max/Move.h>

using namespace sa::core;
using namespace sa::targets::salesman_area_max;

double SalesmanAreaMaxPosition::getEnergy() const { return energy; }

IMove::CPtr SalesmanAreaMaxPosition::generateMove() const
{
  if (cities.size() < 2) {
    return std::make_unique<SalesmanAreaMaxMove>(0, 0, 0, 0);
  }
  std::size_t idx = r.randomInt(0, cities.size() - 1);
  std::size_t shiftIdx = r.randomInt(0, cities.size() - 1);
  const auto& [prevIdx, nextIdx] = getNeighbourIdxs(idx);
  const auto& [prevShiftIdx, nextShiftIdx] = getNeighbourIdxs(shiftIdx);
  if ((idx == shiftIdx) || (nextIdx == shiftIdx)) {
    return std::make_unique<SalesmanAreaMaxMove>(idx, shiftIdx, 0, 0);
  }
  double signedDelta = shoelace(cities[prevIdx], cities[nextIdx]) + shoelace(cities[prevShiftIdx], cities[idx]) +
                       shoelace(cities[idx], cities[shiftIdx]) - shoelace(cities[prevIdx], cities[idx]) -
                       shoelace(cities[idx], cities[nextIdx]) - shoelace(cities[prevShiftIdx], cities[shiftIdx]);
  double delta = -std::abs(signedEnergy + signedDelta) - energy;
  return std::make_unique<SalesmanAreaMaxMove>(idx, shiftIdx, delta, signedDelta);
}

void SalesmanAreaMaxPosition::makeMove(IMove::CPtr move)
{
  auto* m = dynamic_cast<SalesmanAreaMaxMove*>(move.get());
  energy += m->getDelta();
  signedEnergy += m->signedDelta;
  std::size_t idx = m->idx;
  std::size_t shiftIdx = m->shiftIdx;
  auto element = cities[idx];
  cities.erase(cities.begin() + idx);
  if (shiftIdx > idx) {
    shiftIdx--;
  }
  cities.insert(cities.begin() + shiftIdx, element);
}

int SalesmanAreaMaxPosition::size() const
{
  return 2 * sizeof(double) + sizeof(std::vector<std::pair<double, double>>) +
         sizeof(std::pair<double, double>) * cities.capacity();
}

IPosition::CPtr SalesmanAreaMaxPosition::clone() const
{
  return std::make_unique<SalesmanAreaMaxPosition>(signedEnergy, cities);
}

std::pair<std::size_t, std::size_t> SalesmanAreaMaxPosition::getNeighbourIdxs(std::size_t idx) const
{
  std::size_t prevIdx = (idx == 0) ? (cities.size() - 1) : idx - 1;
  std::size_t nextIdx = (idx == (cities.size() - 1)) ? 0 : idx + 1;
  return {prevIdx, nextIdx};
}

double SalesmanAreaMaxPosition::shoelace(const std::pair<double, double>& city1,
                                         const std::pair<double, double>& city2) const
{
  const auto& [x1, y1] = city1;
  const auto& [x2, y2] = city2;
  return (x1 * y2 - x2 * y1) / 2.0;
}

double SalesmanAreaMaxPosition::calcSignedEnergy() const
{
  if (cities.size() <= 2) {
    return 0;
  }
  double ret = 0;
  for (std::size_t idx = 0; idx < cities.size(); ++idx) {
    const auto& [prevIdx, nextIdx] = getNeighbourIdxs(idx);
    ret += shoelace(cities[idx], cities[nextIdx]);
  }
  return ret;
}

std::string SalesmanAreaMaxPosition::toString(const IPosition::CPtr& iPosition)
{
  SalesmanAreaMaxPosition* position = dynamic_cast<SalesmanAreaMaxPosition*>(iPosition.get());
  std::stringstream ss;
  ss << std::setprecision(Rounding::precision) << std::fixed;
  ss << getTypeId() << " ";
  for (const auto& [d1, d2] : position->cities) {
    ss << d1 << " " << d2 << " ";
  }
  return ss.str();
}

IPosition::CPtr SalesmanAreaMaxPosition::fromString(const std::string& data)
{
  std::istringstream ss(data);
  std::string typeId;
  ss >> typeId;

  std::vector<std::pair<double, double>> cities;
  double d1, d2;
  while (ss >> d1 >> d2) {
    cities.emplace_back(Rounding::roundDouble(d1), Rounding::roundDouble(d2));
  }
  auto position = std::make_unique<SalesmanAreaMaxPosition>(std::move(cities));
  return position;
}

std::string SalesmanAreaMaxPosition::getTypeId() { return "salesman_area_max"; }

void SalesmanAreaMaxPosition::plot() const
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
