#include <gmock/gmock.h>

#include <cmath>
#include <memory>

#include <core/IPosition.h>
#include <salesman/Position.h>

using namespace sa::core;
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
  std::mt19937 mt(0);
  std::uniform_real_distribution<> dist(0.0, 1.0);
  std::vector<std::pair<double, double>> ret;
  for (int i = 0; i < n; ++i) {
    double x = dist(mt);
    double y = dist(mt);
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
  std::shared_ptr<IMove> m = std::make_shared<SalesmanMove>(m1, m2);
  auto p2 = p.makeMove(m);
  auto p3 = std::dynamic_pointer_cast<SalesmanPosition>(p2);
  if (m2 < m1) {
    std::swap(m1, m2);
  }
  std::size_t tested = 0;
  for (int i = 0; i < m1; ++i) {
    EXPECT_TRUE(isEqual(cities[i], p3->cities[i]));
    ++tested;
  }
  for (int i = 0; i < m2 - m1 + 1; ++i) {
    EXPECT_TRUE(isEqual(cities[m1 + i], p3->cities[m2 - i]));
    ++tested;
  }
  for (std::size_t i = m2 + 1; i < cities.size(); ++i) {
    EXPECT_TRUE(isEqual(cities[i], p3->cities[i]));
    ++tested;
  }
  EXPECT_EQ(cities.size(), p3->cities.size());
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
  std::shared_ptr<IPosition> curr = std::make_shared<SalesmanPosition>(cities);
  std::vector<std::shared_ptr<IMove>> moves;
  for (int i = 0; i < m; ++i) {
    auto move = curr->getMove();
    moves.push_back(move);
    curr = curr->makeMove(move);
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
