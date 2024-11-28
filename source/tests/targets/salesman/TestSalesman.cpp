#include <gmock/gmock.h>

#include <salesman/Position.h>

using namespace sa::core;
using namespace sa::targets::salesman;

TEST(Salersman, Energy)
{
  std::vector<std::pair<double, double>> cities_ = {{0, 0}, {0, 1}, {1, 1}, {1, 0}};
  SalesmanPosition p(cities_);
  double energy = p.getEnergy();
  EXPECT_NEAR(4.0, energy, 1e-9);
}
