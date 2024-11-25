#include <iostream>

#include <policies/Acceptance.h>
#include <policies/Cooling.h>
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
    SA sa1(Iteration(5000), Metropolis(), Linear());
    SA sa2(Iteration(100000), Metropolis(), Linear());
    SA sa3(Iteration(10000000), Metropolis(), Linear());
    sa1.anneal(position);
    std::cout << "idx=" << idx << " currEnergy=" << sa1.currEnergy << " upEnergyChanges=" << sa1.upEnergyChanges << " "
              << sa1.toString() << "\n";
    sa2.anneal(position);
    std::cout << "idx=" << idx << " currEnergy=" << sa2.bestEnergy << " upEnergyChanges=" << sa2.upEnergyChanges << " "
              << sa2.toString() << "\n";
    sa3.anneal(position);
    std::cout << "idx=" << idx << " currEnergy=" << sa3.bestEnergy << " upEnergyChanges=" << sa3.upEnergyChanges << " "
              << sa3.toString() << "\n";
    SA sa4(Time(5), Metropolis(), Linear());
    sa4.anneal(position);
    std::cout << "idx=" << idx << " currEnergy=" << sa4.bestEnergy << " upEnergyChanges=" << sa4.upEnergyChanges << " "
              << sa4.toString() << "\n";
    SA sa5(Time(5), Metropolis(), Quadratic());
    sa5.anneal(position);
    std::cout << "idx=" << idx << " currEnergy=" << sa5.bestEnergy << " upEnergyChanges=" << sa5.upEnergyChanges << " "
              << sa5.toString() << "\n";              
    ++idx;
  }
  return 0;
}
