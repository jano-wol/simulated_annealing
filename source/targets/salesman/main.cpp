#include <sa/sa.h>
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
  auto positions = generateTestCases();
  std::size_t idx = 0;
  for (const auto& position : positions) {
    // print(position);
    sa.anneal(position, 5000, 1.0, 1.0);
    std::cout << "idx=" << idx << " currEnergy=" << sa.currEnergy << " upEnergyChanges=" << sa.upEnergyChanges << "\n";
    sa.anneal(position, 100000, 1.0, 1.0);
    std::cout << "idx=" << idx << " currEnergy=" << sa.bestEnergy << " upEnergyChanges=" << sa.upEnergyChanges << "\n";
    sa.anneal(position, 10000000, 1.0, 1.0);
    std::cout << "idx=" << idx << " currEnergy=" << sa.bestEnergy << " upEnergyChanges=" << sa.upEnergyChanges << "\n";
    ++idx;
  }
  return 0;
}
