#include <gmock/gmock.h>

#include <cmath>
#include <memory>

#include <core/IPosition.h>
#include <core/Random.h>
#include <policies/Acceptance.h>
#include <policies/Cooling.h>
#include <policies/Resource.h>
#include <sa/sa.h>
#include <salesman/Position.h>
#include <salesman/Tester.h>

using namespace sa::core;
using namespace sa::policies;
using namespace sa::sa;
using namespace sa::targets::salesman;

namespace
{
double delta = 1e-9;

void testEnergy(std::vector<std::pair<double, double>> cities, double expected)
{
  SalesmanPosition p(cities);
  double energy = p.getEnergy();
  EXPECT_NEAR(expected, energy, delta);
}

bool isEqual(const std::pair<double, double>& p1, const std::pair<double, double>& p2)
{
  return std::abs(p1.first - p2.first) < delta && std::abs(p1.second - p2.second) < delta;
}

std::vector<std::pair<double, double>> getRandomCities(int n)
{
  std::vector<std::pair<double, double>> ret;
  for (int i = 0; i < n; ++i) {
    double x = sa::core::Random::randomDouble(0, 1);
    double y = sa::core::Random::randomDouble(0, 1);
    ret.push_back({x * 10.0, y * 10.0});
  }

  for (int i = 0; i < n; ++i) {
    int count = 0;
    for (int j = 0; j < n; ++j) {
      if (isEqual(ret[i], ret[j])) {
        ++count;
      }
    }
    EXPECT_EQ(count, 1);
  }

  return ret;
}

void testMove(const std::vector<std::pair<double, double>>& cities, int m1, int m2)
{
  SalesmanPosition p(cities);
  IMove::Ptr m = std::make_shared<SalesmanMove>(m1, m2);
  auto n_ = p.createNeighbour(m);
  auto n = std::dynamic_pointer_cast<SalesmanPosition>(n_);
  if (m2 < m1) {
    std::swap(m1, m2);
  }
  std::size_t tested = 0;
  for (int i = 0; i < m1; ++i) {
    EXPECT_TRUE(isEqual(cities[i], n->cities[i]));
    ++tested;
  }
  for (int i = 0; i < m2 - m1 + 1; ++i) {
    EXPECT_TRUE(isEqual(cities[m1 + i], n->cities[m2 - i]));
    ++tested;
  }
  for (std::size_t i = m2 + 1; i < cities.size(); ++i) {
    EXPECT_TRUE(isEqual(cities[i], n->cities[i]));
    ++tested;
  }
  EXPECT_EQ(cities.size(), n->cities.size());
  EXPECT_EQ(cities.size(), tested);
}

}  // namespace

TEST(Salesman, Energy)
{
  std::vector<std::pair<double, double>> cities0;
  std::vector<std::pair<double, double>> cities1 = {{1, 1}};
  std::vector<std::pair<double, double>> cities2 = {{0, 0}, {0, 1}};
  std::vector<std::pair<double, double>> cities3 = {{0, 0}, {0, 3}, {4, 0}};
  std::vector<std::pair<double, double>> cities3_2 = {{0, 0}, {0, 1}, {1, 0}};
  std::vector<std::pair<double, double>> cities4 = {{0, 0}, {0, 1}, {1, 1}, {1, 0}};
  testEnergy(cities0, 0);
  testEnergy(cities1, 0);
  testEnergy(cities2, 2);
  testEnergy(cities3, 12);
  testEnergy(cities3_2, 2 + std::sqrt(2));
  testEnergy(cities4, 4);
}

TEST(Salesman, Move)
{
  std::vector<std::pair<double, double>> cities = {{1, 0}, {2, 0}, {3, 1}, {3, 2}, {2, 3}, {1, 3}, {0, 2}, {0, 1}};
  for (int i = 0; i < 8; ++i) {
    for (int j = 0; j < 8; ++j) {
      testMove(cities, i, j);
    }
  }
}

TEST(Salesman, MoveRand)
{
  int n = 20;
  int m = 100;
  auto cities = getRandomCities(n);
  IPosition::Ptr curr = std::make_shared<SalesmanPosition>(cities);
  std::vector<IMove::Ptr> moves;
  for (int i = 0; i < m; ++i) {
    auto move = curr->generateMove();
    moves.push_back(move);
    curr = curr->createNeighbour(move);
  }
  auto endPosition = std::dynamic_pointer_cast<SalesmanPosition>(curr);
  auto cities2 = endPosition->cities;
  for (const auto& city : cities2) {
    int count = 0;
    for (int i = 0; i < n; ++i) {
      if (isEqual(city, cities[i])) {
        ++count;
      }
    }
    EXPECT_EQ(count, 1);
  }
  int fixed = 0;
  for (int i = 0; i < n; ++i) {
    if (isEqual(cities[i], cities2[i])) {
      ++fixed;
    }
  }
  EXPECT_NE(fixed, n);
}

TEST(Salesman, Fast)
{
  int n = 14;
  int m = 1000;
  auto cities = getRandomCities(n);
  IPosition::Ptr currSlow = std::make_shared<SalesmanPosition>(cities);
  IPosition::Ptr currFast = std::make_shared<SalesmanPosition>(cities);
  double fastEnergy = currFast->getEnergy();
  std::vector<IMove::Ptr> moves;
  for (int i = 0; i < m; ++i) {
    EXPECT_NEAR(currSlow->getEnergy(), fastEnergy, delta);
    auto move = currSlow->generateMove();
    currSlow = currSlow->createNeighbour(move);
    auto d = currFast->getDelta(move);
    currFast->makeMove(move);
    fastEnergy += *d;
  }
}

TEST(Salesman, Annealing)
{
  int n = 20;
  auto cities = getRandomCities(n);
  SA sa(std::make_unique<Iteration>(1000), std::make_unique<Metropolis>(), std::make_unique<Linear>());
  IPosition::Ptr position = std::make_shared<SalesmanPosition>(cities);
  double startEnergy = position->getEnergy();
  sa.anneal(position);
  ASSERT_LE(sa.bestEnergy, startEnergy);
  ASSERT_LE(0, sa.bestEnergy);
}
