#ifndef SIMULATED_ANNEALING_LIB_TARGETS_SALESMAN_TESTER_H_
#define SIMULATED_ANNEALING_LIB_TARGETS_SALESMAN_TESTER_H_

#include <salesman/Position.h>

namespace sa::targets::salesman
{
std::vector<std::shared_ptr<core::IPosition>> getStartPositions(std::size_t numPositions, std::size_t numCities,
                                                                double length)
{
  std::mt19937 mt = std::mt19937(0);
  auto dist = std::uniform_real_distribution<>(0.0, 1.0);
  std::vector<std::shared_ptr<core::IPosition>> ret;
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

std::vector<std::shared_ptr<core::IPosition>> getStartPositions_5x5(std::size_t numPositions, std::size_t numCities,
                                                                    double length)
{
  std::mt19937 mt = std::mt19937(0);
  auto dist = std::uniform_real_distribution<>(0.0, 1.0);
  auto dist2 = std::uniform_int_distribution<>(0, 4);

  std::vector<int> chosenSubsquares;
  std::vector<int> chosenSubsquaresDensity;
  chosenSubsquares.push_back(0);
  for (int i = 1; i < 25; ++i) {
    int r = dist2(mt);
    if (r == 0) {
      chosenSubsquares.push_back(0);
    }
  }

  for (auto square : chosenSubsquares) {
    int r = dist2(mt) + 1;
    for (int i = 0; i < r; ++i) {
      chosenSubsquaresDensity.push_back(square);
    }
  }
  auto dist3 = std::uniform_int_distribution<>(0, chosenSubsquaresDensity.size() - 1);
  std::vector<std::shared_ptr<core::IPosition>> ret;
  ret.reserve(numPositions);
  for (std::size_t idx = 0; idx < numPositions; ++idx) {
    std::vector<std::pair<double, double>> cities;
    cities.reserve(numCities);
    for (std::size_t c = 0; c < numCities; ++c) {
      int r = dist3(mt);
      int sq = chosenSubsquaresDensity[r];
      int x = sq / 5;
      int y = sq % 5;
      double randomResult1 = dist(mt);
      double randomResult2 = dist(mt);
      cities.push_back({(x + randomResult1) * length, (y + randomResult2) * length});
    }
    auto startPosition = std::make_shared<SalesmanPosition>(cities);
    ret.push_back(startPosition);
  }
  return ret;
}

std::vector<std::shared_ptr<core::IPosition>> generateTestCases()
{
  std::vector<std::shared_ptr<core::IPosition>> r;
  std::vector<int> nc{5, 10, 20, 50, 100, 200, 500, 1000};
  std::vector<double> l{1.0, 2.0, 5.0, 10.0, 50.0, 100.0, 100.0, 100.0};
  for (int i = 0; i < 8; ++i) {
    auto x = getStartPositions(1, nc[i], l[i])[0];
    auto y = getStartPositions_5x5(1, nc[i], l[i])[0];
    r.push_back(x);
    r.push_back(y);
  }
  return r;
}
}  // namespace sa::targets::salesman

#endif  // SIMULATED_ANNEALING_LIB_TARGETS_SALESMAN_TESTER_H_
