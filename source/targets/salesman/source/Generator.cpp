#include <sstream>

#include <core/Random.h>
#include <core/Rounding.h>
#include <salesman/Generator.h>
#include <salesman/Position.h>

using namespace sa::core;
using namespace sa::targets::salesman;

IPosition::CPtr SalesmanGenerator::getStartPosition(int idx) const
{
  Random r(idx);
  std::vector<std::pair<double, double>> cities;
  cities.reserve(numCities);
  for (int c = 0; c < numCities; ++c) {
    double randomResult1 = r.randomDouble(0.0, 1.0);
    double randomResult2 = r.randomDouble(0.0, 1.0);
    cities.push_back({Rounding::roundDouble(length * randomResult1), Rounding::roundDouble(length * randomResult2)});
  }
  return std::make_unique<SalesmanPosition>(cities);
}

IPosition::CPtr SalesmanGenerator::getStartPosition_5x5(int idx) const
{
  Random r(idx);
  std::vector<int> chosenSubsquares;
  std::vector<int> chosenSubsquaresDensity;
  chosenSubsquares.push_back(0);
  chosenSubsquaresDensity.push_back(0);
  for (int i = 1; i < 25; ++i) {
    int rr = r.randomInt(0, 4);
    if (rr == 0) {
      chosenSubsquares.push_back(i);
    }
  }

  for (auto square : chosenSubsquares) {
    int rr = r.randomInt(0, 4);
    for (int i = 0; i < rr; ++i) {
      chosenSubsquaresDensity.push_back(square);
    }
  }
  std::vector<std::pair<double, double>> cities;
  cities.reserve(numCities);
  for (int c = 0; c < numCities; ++c) {
    int rr = r.randomInt(0, chosenSubsquaresDensity.size() - 1);
    int sq = chosenSubsquaresDensity[rr];
    int x = sq / 5;
    int y = sq % 5;
    double randomResult1 = r.randomDouble(0.0, 1.0);
    double randomResult2 = r.randomDouble(0.0, 1.0);
    cities.push_back(
        {Rounding::roundDouble((x + randomResult1) * length), Rounding::roundDouble((y + randomResult2) * length)});
  }
  return std::make_unique<SalesmanPosition>(cities);
}

IPosition::CPtr SalesmanGenerator::generatePosition(int idx) const
{
  if (subSquares) {
    return getStartPosition_5x5(idx);
  } else {
    return getStartPosition(idx);
  }
}

std::string SalesmanGenerator::getGeneratorName() const
{
  std::stringstream ss;
  ss << numCities << "_" << length;
  if (subSquares) {
    ss << "_5x5";
  }
  return ss.str();
}
