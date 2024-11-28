#include <gmock/gmock.h>

#include <cmath>

#include <salesman/Position.h>

using namespace sa::core;
using namespace sa::targets::salesman;

namespace
{
void testEnergy(std::vector<std::pair<double, double>> cities, double expected)
{
  SalesmanPosition p(cities);
  double energy = p.getEnergy();
  EXPECT_NEAR(expected, energy, 1e-9);
}
}  // namespace

TEST(Salersman, Energy)
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
