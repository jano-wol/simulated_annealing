#ifndef SIMULATED_ANNEALING_TARGETS_SALESMAN_SALESMANPOSITION_H_
#define SIMULATED_ANNEALING_TARGETS_SALESMAN_SALESMANPOSITION_H_

#include <sa/IPosition.h>
#include <cmath>
#include <random>
#include <vector>
#include "salesman_move.h"

using namespace sa::core;

class SalesmanPosition : public IPosition
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

  std::optional<double> getEnergyFast(const std::shared_ptr<IMove>& /*imove*/, double /*baseEnergy*/) const override
  {
    return std::nullopt;
  }

  std::shared_ptr<IMove> getMove() const override
  {
    std::uniform_int_distribution<> dist(0, cities.size() - 1);
    std::uniform_int_distribution<> distInner(0, 1);
    std::size_t cityIdx1 = dist(mt);
    std::size_t cityIdx2 = dist(mt);
    bool inner = distInner(mt);
    return std::make_shared<SalesmanMove>(cityIdx1, cityIdx2, inner);
  }

  void makeMoveInplace(const std::shared_ptr<IMove>& imove) override
  {
    auto move = std::dynamic_pointer_cast<SalesmanMove>(imove);
    std::size_t cityIdx1 = move->cityIdx1;
    std::size_t cityIdx2 = move->cityIdx2;
    bool inner = move->inner;
    if (cityIdx1 == cityIdx2) {
      return;
    }
    auto newCities = cities;
    newCities[cityIdx1] = cities[cityIdx2];
    newCities[cityIdx2] = cities[cityIdx1];
    std::size_t backCityIdx;
    std::size_t forCityIdx;
    if (cityIdx1 < cityIdx2) {
      backCityIdx = cityIdx1;
      forCityIdx = cityIdx2;
    } else {
      backCityIdx = cityIdx2;
      forCityIdx = cityIdx1;
    }
    if (inner) {
      std::size_t j = 1;
      for (std::size_t i = forCityIdx - 1; i > forCityIdx; --i) {
        newCities[backCityIdx + j] = cities[i];
      }
    } else {
      std::size_t s = cities.size();
      std::size_t citiesIdx = (forCityIdx + 1) % s;
      std::size_t newCitiesIdx = (backCityIdx + s - 1) % s;
      for (std::size_t i = 0; i < s - (forCityIdx - backCityIdx + 1); ++i) {
        newCities[newCitiesIdx] = cities[citiesIdx];
        citiesIdx = (citiesIdx + 1) % s;
        newCitiesIdx = (newCitiesIdx + s - 1) % s;
      }
    }
    cities = newCities;
  }

  std::shared_ptr<IPosition> makeMove(const std::shared_ptr<IMove>& imove) const override
  {
    auto cities_ = cities;
    auto ret = std::make_shared<SalesmanPosition>(std::move(cities_));
    ret->makeMoveInplace(imove);
    return ret;
  }

  std::vector<std::pair<double, double>> cities;
  static std::mt19937 mt;
};

#endif  // SIMULATED_ANNEALING_TARGETS_SALESMAN_SALESMANPOSITION_H_
