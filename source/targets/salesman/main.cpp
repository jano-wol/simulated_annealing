#include <policies/iteration_policy.h>
#include <sa/sa.h>
#include <iostream>
#include "salesman_position.h"
#include "tester.h"

using namespace sa::policies;
using namespace sa::sa;

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
  SA sa1(IterationPolicy(5000));
  SA sa2(IterationPolicy(100000));
  SA sa3(IterationPolicy(10000000));
  auto positions = generateTestCases();
  std::size_t idx = 0;
  for (const auto& position : positions) {
    // print(position);
    sa1.anneal(position, 5000, 1.0, 1.0);
    std::cout << "idx=" << idx << " currEnergy=" << sa1.currEnergy << " upEnergyChanges=" << sa1.upEnergyChanges
              << "\n";
    sa2.anneal(position, 100000, 1.0, 1.0);
    std::cout << "idx=" << idx << " currEnergy=" << sa2.bestEnergy << " upEnergyChanges=" << sa2.upEnergyChanges
              << "\n";
    sa3.anneal(position, 10000000, 1.0, 1.0);
    std::cout << "idx=" << idx << " currEnergy=" << sa3.bestEnergy << " upEnergyChanges=" << sa3.upEnergyChanges
              << "\n";
    ++idx;
  }
  return 0;
}
