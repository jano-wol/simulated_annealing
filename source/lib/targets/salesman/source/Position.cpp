#include <salesman/Position.h>

#include <cmath>

#include <core/IPosition.h>
#include <core/Random.h>
#include <salesman/Move.h>

using namespace sa::core;
using namespace sa::targets::salesman;

double SalesmanPosition::getEnergy() const { return energy; }

IMove::CPtr SalesmanPosition::generateMove() const
{
  std::size_t idx1 = r.randomInt(0, cities.size() - 1);
  std::size_t idx2 = r.randomInt(0, cities.size() - 1);
  if (idx1 == idx2) {
    return std::make_unique<SalesmanMove>(idx1, idx2, 0);
  }
  if (idx2 < idx1) {
    std::swap(idx1, idx2);
  }
  if (idx1 == 0 && idx2 == cities.size() - 1) {
    return std::make_unique<SalesmanMove>(idx1, idx2, 0);
  }
  const auto& [prevIdx1, nextIdx1] = getNeighbourIdxs(idx1);
  const auto& [prevIdx2, nextIdx2] = getNeighbourIdxs(idx2);
  double delta = distance(cities[idx1], cities[nextIdx2]) + distance(cities[idx2], cities[prevIdx1]) -
                 distance(cities[idx1], cities[prevIdx1]) - distance(cities[idx2], cities[nextIdx2]);
  return std::make_unique<SalesmanMove>(idx1, idx2, delta);
}

void SalesmanPosition::makeMove(IMove::CPtr move)
{
  auto* m = dynamic_cast<SalesmanMove*>(move.get());
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

int SalesmanPosition::size() const
{
  return sizeof(double) + sizeof(std::vector<std::pair<double, double>>) +
         sizeof(std::pair<double, double>) * cities.capacity();
}

IPosition::CPtr SalesmanPosition::clone() const { return std::make_unique<SalesmanPosition>(energy, cities, r); }

std::pair<std::size_t, std::size_t> SalesmanPosition::getNeighbourIdxs(std::size_t idx) const
{
  std::size_t prevIdx = (idx == 0) ? (cities.size() - 1) : idx - 1;
  std::size_t nextIdx = (idx == (cities.size() - 1)) ? 0 : idx + 1;
  return {prevIdx, nextIdx};
}

double SalesmanPosition::distance(const std::pair<double, double>& city1, const std::pair<double, double>& city2) const
{
  const auto& [x1, y1] = city1;
  const auto& [x2, y2] = city2;
  double ret = ((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
  return std::sqrt(ret);
}

double SalesmanPosition::calcEnergy() const
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
