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

double makeEnergyPenality(double angle) { return std::abs((std::numbers::pi - angle)); }

double SalesmanAnglePosition::getEnergy() const { return energy; }

IMove::CPtr SalesmanAnglePosition::generateMove() const
{
  if (cities.size() < 4) {
    return std::make_unique<SalesmanAngleMove>(0, 0, 0);
  }
  std::size_t idx = r.randomInt(0, cities.size() - 1);
  std::size_t shiftIdx = r.randomInt(0, cities.size() - 1);
  const auto& [prevIdx, nextIdx] = getNeighbourIdxs(idx);
  const auto& [prevShiftIdx, nextShiftIdx] = getNeighbourIdxs(shiftIdx);
  double delta = 0;
  if (idx == shiftIdx || nextIdx == shiftIdx) {
    return std::make_unique<SalesmanAngleMove>(idx, shiftIdx, 0);
  }
  if (getNeighbourIdxs(nextIdx).second == shiftIdx) {
    double deltaNew = makeEnergyPenality(angle(prevIdx, getNeighbourIdxs(prevIdx).first, nextIdx)) +
                      makeEnergyPenality(angle(nextIdx, prevIdx, idx)) +
                      makeEnergyPenality(angle(idx, prevShiftIdx, shiftIdx)) +
                      makeEnergyPenality(angle(shiftIdx, idx, nextShiftIdx));
    double deltaOld = makeEnergyPenality(angle(prevIdx)) + makeEnergyPenality(angle(idx)) +
                      makeEnergyPenality(angle(nextIdx)) + makeEnergyPenality(angle(shiftIdx));
    delta = deltaNew - deltaOld;
  } else if (nextShiftIdx == idx) {
    double deltaNew = makeEnergyPenality(angle(prevShiftIdx, getNeighbourIdxs(prevShiftIdx).first, idx)) +
                      makeEnergyPenality(angle(nextIdx, prevIdx, getNeighbourIdxs(nextIdx).second)) +
                      makeEnergyPenality(angle(idx, prevShiftIdx, shiftIdx)) +
                      makeEnergyPenality(angle(shiftIdx, idx, nextIdx));
    double deltaOld = makeEnergyPenality(angle(prevIdx)) + makeEnergyPenality(angle(idx)) +
                      makeEnergyPenality(angle(nextIdx)) + makeEnergyPenality(angle(prevShiftIdx));
    delta = deltaNew - deltaOld;
  } else {
    double deltaNew = makeEnergyPenality(angle(prevIdx, getNeighbourIdxs(prevIdx).first, nextIdx)) +
                      makeEnergyPenality(angle(nextIdx, prevIdx, getNeighbourIdxs(nextIdx).second)) +
                      makeEnergyPenality(angle(idx, prevShiftIdx, shiftIdx)) +
                      makeEnergyPenality(angle(shiftIdx, idx, nextShiftIdx)) +
                      makeEnergyPenality(angle(prevShiftIdx, getNeighbourIdxs(prevShiftIdx).first, idx));
    double deltaOld = makeEnergyPenality(angle(prevIdx)) + makeEnergyPenality(angle(idx)) +
                      makeEnergyPenality(angle(nextIdx)) + makeEnergyPenality(angle(shiftIdx)) +
                      makeEnergyPenality(angle(prevShiftIdx));
    delta = deltaNew - deltaOld;
  }
  return std::make_unique<SalesmanAngleMove>(idx, shiftIdx, delta);
}

void SalesmanAnglePosition::makeMove(IMove::CPtr move)
{
  auto* m = dynamic_cast<SalesmanAngleMove*>(move.get());
  energy += m->getDelta();
  std::size_t idx = m->idx;
  std::size_t shiftIdx = m->shiftIdx;
  auto element = cities[idx];
  cities.erase(cities.begin() + idx);
  if (shiftIdx > idx) {
    shiftIdx--;
  }
  cities.insert(cities.begin() + shiftIdx, element);
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
    double a = angle(idx);
    double e = makeEnergyPenality(a);
    ret += e;
  }
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
