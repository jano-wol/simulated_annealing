#include <gmock/gmock.h>

#include <cmath>
#include <memory>

#include <core/IPosition.h>
#include <core/Random.h>
#include <monitor/Monitor.h>
#include <policies/Acceptance.h>
#include <policies/Cooling.h>
#include <policies/Resource.h>
#include <sa/SA.h>
#include <salesman/Position.h>
#include <salesman/Tester.h>
#include <serializator/Serializator.h>

using namespace sa::core;
using namespace sa::monitor;
using namespace sa::policies;
using namespace sa::sa;
using namespace sa::serializator;
using namespace sa::targets::salesman;

namespace
{
double precision = 1e-9;

void testEnergy(std::vector<std::pair<double, double>> cities, double expected)
{
  SalesmanPosition p(cities);
  double energy = p.getEnergy();
  EXPECT_NEAR(expected, energy, precision);
}

bool isEqual(const std::pair<double, double>& p1, const std::pair<double, double>& p2)
{
  return std::abs(p1.first - p2.first) < precision && std::abs(p1.second - p2.second) < precision;
}

std::vector<std::pair<double, double>> getRandomCities(int n)
{
  Random r;
  std::vector<std::pair<double, double>> ret;
  for (int i = 0; i < n; ++i) {
    double x = r.randomDouble(0, 1);
    double y = r.randomDouble(0, 1);
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
  IMove::CPtr m = std::make_unique<SalesmanMove>(m1, m2, 0);
  p.makeMove(std::move(m));
  if (m2 < m1) {
    std::swap(m1, m2);
  }
  std::size_t tested = 0;
  for (int i = 0; i < m1; ++i) {
    EXPECT_TRUE(isEqual(cities[i], p.cities[i]));
    ++tested;
  }
  for (int i = 0; i < m2 - m1 + 1; ++i) {
    EXPECT_TRUE(isEqual(cities[m1 + i], p.cities[m2 - i]));
    ++tested;
  }
  for (std::size_t i = m2 + 1; i < cities.size(); ++i) {
    EXPECT_TRUE(isEqual(cities[i], p.cities[i]));
    ++tested;
  }
  EXPECT_EQ(cities.size(), p.cities.size());
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
  IPosition::CPtr curr = std::make_unique<SalesmanPosition>(cities);
  for (int i = 0; i < m; ++i) {
    auto move = curr->generateMove();
    curr->makeMove(std::move(move));
  }
  auto* endPosition = dynamic_cast<SalesmanPosition*>(curr.get());
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
  IPosition::CPtr curr = std::make_unique<SalesmanPosition>(cities);
  for (int i = 0; i < m; ++i) {
    auto* p = dynamic_cast<SalesmanPosition*>(curr.get());
    EXPECT_NEAR(curr->getEnergy(), p->calcEnergy(), precision);
    auto move = curr->generateMove();
    curr->makeMove(std::move(move));
  }
}

TEST(Salesman, Annealing)
{
  int n = 20;
  auto cities = getRandomCities(n);
  SA sa(std::make_unique<Iteration>(1000), std::make_unique<Metropolis>(), std::make_unique<Linear>(),
        std::make_unique<KBest>(1), Monitor(MonitorLevel::Low));
  IPosition::CPtr position = std::make_unique<SalesmanPosition>(cities);
  double startEnergy = position->getEnergy();
  sa.anneal(position);
  ASSERT_LE(sa.currPosition->getEnergy(), startEnergy);
  ASSERT_LE(0, sa.currPosition->getEnergy());
}

TEST(Salesman, Reproducibility)
{
  int n = 100;
  auto cities = getRandomCities(n);
  SA sa1(std::make_unique<Iteration>(10000), std::make_unique<Metropolis>(), std::make_unique<Linear>(),
        std::make_unique<KBest>(1), Monitor(MonitorLevel::Low));
  SA sa2(std::make_unique<Iteration>(10000), std::make_unique<Metropolis>(), std::make_unique<Linear>(),
        std::make_unique<KBest>(1), Monitor(MonitorLevel::Low));
  IPosition::CPtr position = std::make_unique<SalesmanPosition>(cities);
  sa1.anneal(position);
  sa2.anneal(position);
  ASSERT_NEAR(sa1.currPosition->getEnergy(), sa2.currPosition->getEnergy(), 1e-4);
}

TEST(Salesman, Serialization)
{
    int n = 100;
  auto cities = getRandomCities(n);
  IPosition::CPtr position = std::make_unique<SalesmanPosition>(cities);
  auto s1 = Serializator::toString(position);
  auto s2 = Serializator::fromString(s1);
  auto s3 = Serializator::toString(s2);
  EXPECT_EQ(s1, s3);
}
