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

template <typename Resource = Iteration, typename Acceptance = Metropolis, typename Cooling = Linear>
void print(const SA<Resource, Acceptance, Cooling>& sa, size_t idx)
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
    SA sa(Iteration(100000), Metropolis(), Linear());
    sa.anneal(pos);
    print(sa, 1);
  } else {
    std::size_t idx = 0;
    for (const auto& position : positions) {
      SA sa1(Iteration(5000), Metropolis(), Linear());
      SA sa2(Iteration(100000), Metropolis(), Linear());
      SA sa3(Iteration(10000000), Metropolis(), Linear());
      sa1.anneal(position);
      print(sa1, idx);
      sa2.anneal(position);
      print(sa2, idx);
      sa3.anneal(position);
      print(sa3, idx);
      SA sa4(Time(5), Metropolis(), Linear());
      sa4.anneal(position);
      print(sa4, idx);
      SA sa5(Time(5), Metropolis(), Quadratic());
      sa5.anneal(position);
      print(sa5, idx);
      ++idx;
    }
  }
  return 0;
}
