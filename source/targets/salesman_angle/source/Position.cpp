#include <salesman_angle/Position.h>

#include <cmath>
#include <iomanip>
#include <numbers>
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
  std::size_t idx1 = r.randomInt(0, int(cities.size()) - 1);
  std::size_t idx2 = r.randomInt(0, int(cities.size()) - 2);
  if (idx2 >= idx1) {
    ++idx2;
  } else {
    std::swap(idx1, idx2);
  }
  if ((idx1 == 0 && idx2 == cities.size() - 1) || ((idx2 + 2) % cities.size() == idx1)) {
    return std::make_unique<SalesmanAngleMove>(idx1, idx2, 0);
  }
  const auto& [prevIdx1, nextIdx1] = getNeighbourIdxs(idx1);
  const auto& [prevIdx2, nextIdx2] = getNeighbourIdxs(idx2);
  double old1 = std::abs((std::numbers::pi - angle(prevIdx1)));
  double old2 = std::abs((std::numbers::pi - angle(idx1)));
  double old3 = std::abs((std::numbers::pi - angle(idx2)));
  double old4 = std::abs((std::numbers::pi - angle(nextIdx2)));
  double new1 = std::abs((std::numbers::pi - angle(prevIdx1, getNeighbourIdxs(prevIdx1).first, idx2)));
  double new2 = std::abs((std::numbers::pi - angle(idx2, prevIdx1, prevIdx2)));
  double new3 = std::abs((std::numbers::pi - angle(idx1, nextIdx1, nextIdx2)));
  double new4 = std::abs((std::numbers::pi - angle(nextIdx2, idx1, getNeighbourIdxs(nextIdx2).second)));
  double delta = new1 + new2 + new3 + new4 - old1 - old2 - old3 - old4;
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
  return int(sizeof(double) + sizeof(std::vector<std::pair<double, double>>) +
         sizeof(std::pair<double, double>) * cities.capacity());
}

IPosition::CPtr SalesmanAnglePosition::clone() const { return std::make_unique<SalesmanAnglePosition>(energy, cities); }

std::pair<std::size_t, std::size_t> SalesmanAnglePosition::getNeighbourIdxs(std::size_t idx) const
{
  std::size_t prevIdx = (idx == 0) ? (cities.size() - 1) : idx - 1;
  std::size_t nextIdx = (idx == (cities.size() - 1)) ? 0 : idx + 1;
  return {prevIdx, nextIdx};
}

double SalesmanAnglePosition::angle(std::size_t curr, std::size_t prev, std::size_t next) const
{
  const auto& [prevX, prevY] = cities[prev];
  const auto& [currX, currY] = cities[curr];
  const auto& [nextX, nextY] = cities[next];
  double v1x = prevX - currX;
  double v1y = prevY - currY;
  double v2x = nextX - currX;
  double v2y = nextY - currY;
  double dot = v1x * v2x + v1y * v2y;
  double l1 = std::sqrt(v1x * v1x + v1y * v1y);
  double l2 = std::sqrt(v2x * v2x + v2y * v2y);
  double cosTheta = dot / (l1 * l2);
  cosTheta = std::max(-1.0, std::min(1.0, cosTheta));
  double angle = std::acos(cosTheta);
  return angle;
}

double SalesmanAnglePosition::angle(std::size_t idx) const
{
  auto [prevIdx, nextIdx] = getNeighbourIdxs(idx);
  return angle(idx, prevIdx, nextIdx);
}

double SalesmanAnglePosition::calcEnergy() const
{
  if (cities.empty() || cities.size() == 1) {
    return 0;
  }
  double ret = 0;
  for (std::size_t idx = 0; idx < cities.size(); ++idx) {
    double val = std::abs((std::numbers::pi - angle(idx)));
    ret += val;
  }
  return ret;
}

std::string SalesmanAnglePosition::toString(const IPosition::CPtr& iPosition)
{
  SalesmanAnglePosition* position = dynamic_cast<SalesmanAnglePosition*>(iPosition.get());
  std::stringstream ss;
  ss << std::setprecision(Rounding::precision) << std::fixed;
  for (const auto& [d1, d2] : position->cities) {
    ss << d1 << " " << d2 << " ";
  }
  return ss.str();
}

IPosition::CPtr SalesmanAnglePosition::fromString(const std::string& data)
{
  std::istringstream ss(data);
  std::vector<std::pair<double, double>> cities;
  double d1, d2;
  while (ss >> d1 >> d2) {
    cities.emplace_back(Rounding::roundDouble(d1), Rounding::roundDouble(d2));
  }
  auto position = std::make_unique<SalesmanAnglePosition>(std::move(cities));
  return position;
}

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
