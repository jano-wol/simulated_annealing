#ifndef SIMULATED_ANNEALING_TARGETS_SALESMAN_SALESMANPOSITION_H_
#define SIMULATED_ANNEALING_TARGETS_SALESMAN_SALESMANPOSITION_H_

#include <core/IPosition.h>
#include <cmath>
#include <vector>
#include "salesman_move.h"

using namespace sa::core;

class SalesmanPosition : public IPosition
{
public:
  SalesmanPosition(std::vector<std::pair<double, double>> cities_) : cities(std::move(cities_)), mt(0) {}

  std::pair<std::size_t, std::size_t> getNeighbourIdxs(std::size_t idx)
  {
    std::size_t prevIdx = (idx == 0) ? (cities.size() - 1) : idx - 1;
    std::size_t nextIdx = (idx == (cities.size() - 1)) ? 0 : idx + 1;
    return {prevIdx, nextIdx};
  }

  double distance(const std::pair<std::size_t, std::size_t>& city1, const std::pair<std::size_t, std::size_t>& city2)
  {
    const auto& [x1, y1] = city1;
    const auto& [x2, y2] = city2;
    double ret = ((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
    return std::sqrt(ret);
  }

  double getEnergy() override
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

  std::optional<double> getEnergyFast(const std::shared_ptr<IMove>& imove, double baseEnergy) override
  {
    auto move = std::dynamic_pointer_cast<SalesmanMove>(imove);
    std::size_t cityIdx1 = move->cityIdx1;
    std::size_t cityIdx2 = move->cityIdx2;
    const auto& [prevIdx1, nextIdx1] = getNeighbourIdxs(cityIdx1);
    const auto& [prevIdx2, nextIdx2] = getNeighbourIdxs(cityIdx2);
    double oldE = 0;
    oldE += distance(cities[cityIdx1], cities[prevIdx1]);
    oldE += distance(cities[cityIdx1], cities[nextIdx1]);
    oldE += distance(cities[cityIdx2], cities[prevIdx2]);
    oldE += distance(cities[cityIdx2], cities[nextIdx2]);
    double newE = 0;
    newE += distance(cities[cityIdx2], cities[prevIdx1]);
    newE += distance(cities[cityIdx2], cities[nextIdx1]);
    newE += distance(cities[cityIdx1], cities[prevIdx2]);
    newE += distance(cities[cityIdx1], cities[nextIdx2]);
    return baseEnergy - oldE + newE;
  }

  std::shared_ptr<IMove> getMove() override
  {
    std::uniform_int_distribution<> dist(0, cities.size() - 1);
    std::size_t cityIdx1 = dist(mt);
    std::size_t cityIdx2 = dist(mt);
    return std::make_shared<SalesmanMove>(cityIdx1, cityIdx2);
  }

  void makeMoveInplace(const std::shared_ptr<IMove>& imove) override
  {
    auto move = std::dynamic_pointer_cast<SalesmanMove>(imove);
    std::size_t cityIdx1 = move->cityIdx1;
    std::size_t cityIdx2 = move->cityIdx2;
    auto backup = cities[cityIdx1];
    cities[cityIdx1] = cities[cityIdx2];
    cities[cityIdx2] = backup;
  }

  std::shared_ptr<IPosition> makeMove(const std::shared_ptr<IMove>& imove) override
  {
    auto cities_ = cities;
    auto ret = std::make_shared<SalesmanPosition>(std::move(cities_));
    ret->makeMoveInplace(imove);
    return ret;
  }

  std::vector<std::pair<double, double>> cities;
  std::mt19937 mt;
};

#endif  // SIMULATED_ANNEALING_TARGETS_SALESMAN_SALESMANPOSITION_H_
