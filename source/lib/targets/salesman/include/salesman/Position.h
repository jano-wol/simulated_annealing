#ifndef SIMULATED_ANNEALING_LIB_TARGETS_SALESMAN_SALESMANPOSITION_H_
#define SIMULATED_ANNEALING_LIB_TARGETS_SALESMAN_SALESMANPOSITION_H_

#include <cmath>
#include <random>
#include <vector>

#include <core/IPosition.h>

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

  std::optional<double> getEnergyFast(const std::shared_ptr<core::IMove>& /*imove*/,
                                      double /*baseEnergy*/) const override
  {
    return std::nullopt;
  }

  std::shared_ptr<core::IMove> getMove() const override
  {
    std::uniform_int_distribution<> dist(0, cities.size() - 1);
    std::size_t cityIdx1 = dist(mt);
    std::size_t cityIdx2 = dist(mt);
    return std::make_shared<SalesmanMove>(cityIdx1, cityIdx2);
  }

  void makeMoveInplace(const std::shared_ptr<core::IMove>& imove) override
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

  std::shared_ptr<core::IPosition> makeMove(const std::shared_ptr<core::IMove>& imove) const override
  {
    auto cities_ = cities;
    auto ret = std::make_shared<SalesmanPosition>(std::move(cities_));
    ret->makeMoveInplace(imove);
    return ret;
  }

  std::vector<std::pair<double, double>> cities;
  static std::mt19937 mt;
};
}  // namespace sa::targets::salesman

#endif  // SIMULATED_ANNEALING_LIB_TARGETS_SALESMAN_SALESMANPOSITION_H_
