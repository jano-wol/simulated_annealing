#include <core/sa.h>
#include <iostream>
#include "salesman_position.h"
#include "tester.h"

using namespace sa::core;

void print(const std::shared_ptr<IPosition>& iposition)
{
  auto position = std::dynamic_pointer_cast<SalesmanPosition>(iposition);
  const auto& cities = position->cities;
  for (const auto& [x, y] : cities) {
    std::cout << "(" << x << " " << y << ") ";
  }
  std::cout << "\n";
}

int main()
{
  SA sa;
  auto positions = getStartPositions(1, 20, 100.0);
  print(positions[0]);
  sa.anneal(positions[0], 5000, 1.0, 10.0);
  return 0;
}
