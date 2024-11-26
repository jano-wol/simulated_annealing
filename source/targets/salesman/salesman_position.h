#ifndef SIMULATED_ANNEALING_TARGETS_SALESMAN_SALESMANPOSITION_H_
#define SIMULATED_ANNEALING_TARGETS_SALESMAN_SALESMANPOSITION_H_

#include <cmath>
#include <random>
#include <vector>

#include <core/IPosition.h>

#include "salesman_move.h"

class SalesmanPosition : public sa::core::IPosition
{
public:
  SalesmanPosition(std::vector<std::pair<double, double>> cities_) : cities(std::move(cities_))
  {
    ordering.reserve(cities.size());
    for (std::size_t i = 0; i < cities.size(); ++i) {
      ordering.push_back(i);
    }
  }

  SalesmanPosition(std::vector<std::pair<double, double>> cities_, std::vector<std::size_t> ordering_)
      : cities(std::move(cities_)), ordering(std::move(ordering_))
  {}

  std::pair<std::size_t, std::size_t> getNeighbourIdxs(std::size_t idx) const
  {
    std::size_t prevIdx = (idx == 0) ? (cities.size() - 1) : idx - 1;
    std::size_t nextIdx = (idx == (cities.size() - 1)) ? 0 : idx + 1;
    return {prevIdx, nextIdx};
  }

  double distance(std::size_t idx1, std::size_t idx2) const
  {
    const auto& [x1, y1] = cities[ordering[idx1]];
    const auto& [x2, y2] = cities[ordering[idx2]];
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
      ret += distance(idx, nextIdx);
    }
    return ret;
  }

  std::optional<double> getEnergyFast(const std::shared_ptr<sa::core::IMove>& imove, double baseEnergy) const override
  {
    auto move = std::dynamic_pointer_cast<SalesmanMove>(imove);
    std::size_t idx1 = move->idx1;
    std::size_t idx2 = move->idx2;
    const auto& [prevIdx1, nextIdx1] = getNeighbourIdxs(idx1);
    const auto& [prevIdx2, nextIdx2] = getNeighbourIdxs(idx2);
    double subst =
        distance(idx1, prevIdx1) + distance(idx1, nextIdx1) + distance(idx2, prevIdx2) + distance(idx2, nextIdx2);
    double add =
        distance(idx2, prevIdx1) + distance(idx2, nextIdx1) + distance(idx1, prevIdx2) + distance(idx1, nextIdx2);
    return baseEnergy - subst + add;
  }

  std::shared_ptr<sa::core::IMove> getMove() const override
  {
    std::uniform_int_distribution<> dist(0, cities.size() - 1);
    std::size_t idx1 = dist(mt);
    std::size_t idx2 = dist(mt);
    return std::make_shared<SalesmanMove>(idx1, idx2);
  }

  void makeMoveInplace(const std::shared_ptr<sa::core::IMove>& imove) override
  {
    auto move = std::dynamic_pointer_cast<SalesmanMove>(imove);
    std::size_t idx1 = move->idx1;
    std::size_t idx2 = move->idx2;
    std::swap(ordering[idx1], ordering[idx2]);
  }

  std::shared_ptr<sa::core::IPosition> makeMove(const std::shared_ptr<sa::core::IMove>& imove) const override
  {
    auto cities_ = cities;
    auto ordering_ = ordering;
    auto ret = std::make_shared<SalesmanPosition>(std::move(cities_), std::move(ordering_));
    ret->makeMoveInplace(imove);
    return ret;
  }

  std::vector<std::pair<double, double>> cities;
  std::vector<std::size_t> ordering;
  static std::mt19937 mt;
};

#endif  // SIMULATED_ANNEALING_TARGETS_SALESMAN_SALESMANPOSITION_H_
