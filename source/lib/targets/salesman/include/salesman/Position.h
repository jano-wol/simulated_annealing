#ifndef SIMULATED_ANNEALING_LIB_TARGETS_SALESMAN_SALESMANPOSITION_H_
#define SIMULATED_ANNEALING_LIB_TARGETS_SALESMAN_SALESMANPOSITION_H_

#include <cmath>
#include <vector>

#include <core/IPosition.h>
#include <core/Random.h>

#include <salesman/Move.h>

namespace sa::targets::salesman
{
class SalesmanPosition : public core::IPosition
{
public:
  SalesmanPosition(std::vector<std::pair<double, double>> cities_) : cities(std::move(cities_)) {}

  std::pair<std::size_t, std::size_t> getNeighbourIdxs(std::size_t idx) const
  {
    std::size_t prevIdx = (idx == 0) ? (cities.size() - 1) : idx - 1;
    std::size_t nextIdx = (idx == (cities.size() - 1)) ? 0 : idx + 1;
    return {prevIdx, nextIdx};
  }

  double distance(const std::pair<double, double>& city1, const std::pair<double, double>& city2) const
  {
    const auto& [x1, y1] = city1;
    const auto& [x2, y2] = city2;
    double ret = ((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
    return std::sqrt(ret);
  }

  double getEnergy() const override
  {
    if (cities.empty() || cities.size() == 1) {
      return 0;
    }
    double ret = 0;
    for (std::size_t idx = 0; idx < cities.size(); ++idx) {
      const auto& [prevIdx, nextIdx] = getNeighbourIdxs(idx);
      ret += distance(cities[idx], cities[nextIdx]);
    }
    return ret;
  }

  std::optional<double> getDelta(const core::IMove::Ptr& imove) const override
  {
    auto move = std::dynamic_pointer_cast<SalesmanMove>(imove);
    std::size_t idx1 = move->cityIdx1;
    std::size_t idx2 = move->cityIdx2;
    if (idx1 == idx2) {
      return 0;
    }
    if (idx2 < idx1) {
      std::swap(idx1, idx2);
    }
    if (idx1 == 0 && idx2 == cities.size() - 1) {
      return 0;
    }
    const auto& [prevIdx1, nextIdx1] = getNeighbourIdxs(idx1);
    const auto& [prevIdx2, nextIdx2] = getNeighbourIdxs(idx2);
    return distance(cities[idx1], cities[nextIdx2]) + distance(cities[idx2], cities[prevIdx1]) -
           distance(cities[idx1], cities[prevIdx1]) - distance(cities[idx2], cities[nextIdx2]);
  }

  core::IMove::Ptr generateMove() const override
  {
    std::size_t cityIdx1 = core::Random::randomInt(0, cities.size() - 1);
    std::size_t cityIdx2 = core::Random::randomInt(0, cities.size() - 1);
    return std::make_shared<SalesmanMove>(cityIdx1, cityIdx2);
  }

  void makeMove(const core::IMove::Ptr& imove) override
  {
    auto move = std::dynamic_pointer_cast<SalesmanMove>(imove);
    std::size_t cityIdx1 = move->cityIdx1;
    std::size_t cityIdx2 = move->cityIdx2;
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

  core::IPosition::Ptr createNeighbour(const core::IMove::Ptr& imove) const override
  {
    auto cities_ = cities;
    auto ret = std::make_shared<SalesmanPosition>(std::move(cities_));
    ret->makeMove(imove);
    return ret;
  }

  std::vector<std::pair<double, double>> cities;
};
}  // namespace sa::targets::salesman

#endif  // SIMULATED_ANNEALING_LIB_TARGETS_SALESMAN_SALESMANPOSITION_H_
