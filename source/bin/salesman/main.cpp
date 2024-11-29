#include <iostream>

#include <policies/Acceptance.h>
#include <policies/Cooling.h>
#include <policies/Resource.h>
#include <sa/sa.h>
#include <salesman/Position.h>
#include <salesman/Tester.h>

using namespace sa::core;
using namespace sa::policies;
using namespace sa::sa;
using namespace sa::targets::salesman;

void print(const SA& sa, size_t idx)
{
  std::cout << "idx=" << idx << " currEnergy=" << sa.currEnergy << " upEnergyChanges=" << sa.upEnergyChanges << " "
            << sa.toString() << "\n";
}

int main(int argc, char** argv)
{
  std::string mode = 1 < argc ? argv[1] : "";
  auto positions = generateTestCases();

  if (mode == "bench") {
    auto pos = positions.back();
    SA sa(std::make_shared<Iteration>(100000), std::make_shared<Metropolis>(), std::make_shared<Linear>());
    sa.anneal(pos);
    print(sa, 1);
  } else {
    std::size_t idx = 0;
    for (const auto& position : positions) {
      SA sa1(std::make_shared<Iteration>(5000), std::make_shared<Metropolis>(), std::make_shared<Linear>());
      SA sa2(std::make_shared<Iteration>(100000), std::make_shared<Metropolis>(), std::make_shared<Linear>());
      SA sa3(std::make_shared<Iteration>(10000000), std::make_shared<Metropolis>(), std::make_shared<Linear>());
      sa1.anneal(position);
      print(sa1, idx);
      sa2.anneal(position);
      print(sa2, idx);
      sa3.anneal(position);
      print(sa3, idx);
      SA sa4(std::make_shared<Time>(5), std::make_shared<Metropolis>(), std::make_shared<Linear>());
      sa4.anneal(position);
      print(sa4, idx);
      SA sa5(std::make_shared<Time>(5), std::make_shared<Metropolis>(), std::make_shared<Quadratic>());
      sa5.anneal(position);
      print(sa5, idx);
      ++idx;
    }
  }
  return 0;
}
