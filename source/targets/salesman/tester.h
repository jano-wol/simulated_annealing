#ifndef SIMULATED_ANNEALING_TARGETS_SALESMAN_TESTER_H_
#define SIMULATED_ANNEALING_TARGETS_SALESMAN_TESTER_H_

#include "salesman_position.h"

std::vector<std::shared_ptr<IPosition>> getStartPositions(std::size_t numPositions, std::size_t numCities,
                                                          double length)
{
  std::random_device rd;
  std::mt19937 mt = std::mt19937(rd());
  auto dist = std::uniform_real_distribution<>(0.0, 1.0);
  std::vector<std::shared_ptr<IPosition>> ret;
  ret.reserve(numPositions);
  for (std::size_t idx = 0; idx < numPositions; ++idx) {
    std::vector<std::pair<double, double>> cities;
    cities.reserve(numCities);
    for (std::size_t c = 0; c < numCities; ++c) {
      double randomResult1 = dist(mt);
      double randomResult2 = dist(mt);
      cities.push_back({length * randomResult1, length * randomResult2});
    }
    auto startPosition = std::make_shared<SalesmanPosition>(cities);
    ret.push_back(startPosition);
  }
  return ret;
}

#endif  // SIMULATED_ANNEALING_TARGETS_SALESMAN_TESTER_H_
