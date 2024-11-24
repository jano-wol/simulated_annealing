#include <iostream>

#include <policies/Acceptance.h>
#include <policies/Resource.h>
#include <sa/sa.h>

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
  auto positions = generateTestCases();
  std::size_t idx = 0;
  for (const auto& position : positions) {
    SA sa1(Iteration(5000), Metropolis());
    SA sa2(Iteration(100000), Metropolis());
    SA sa3(Iteration(10000000), Metropolis());
    sa1.anneal(position, 1.0);
    std::cout << "idx=" << idx << " currEnergy=" << sa1.currEnergy << " upEnergyChanges=" << sa1.upEnergyChanges
              << "\n";
    sa2.anneal(position, 1.0);
    std::cout << "idx=" << idx << " currEnergy=" << sa2.bestEnergy << " upEnergyChanges=" << sa2.upEnergyChanges
              << "\n";
    sa3.anneal(position, 1.0);
    std::cout << "idx=" << idx << " currEnergy=" << sa3.bestEnergy << " upEnergyChanges=" << sa3.upEnergyChanges
              << "\n";
    SA sa4(Time(5), Metropolis());
    sa4.anneal(position, 1.0);
    std::cout << "idx=" << idx << " currEnergy=" << sa4.bestEnergy << " upEnergyChanges=" << sa4.upEnergyChanges
              << "\n";
    ++idx;
  }
  return 0;
}
