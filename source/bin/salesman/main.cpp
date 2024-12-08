#include <iostream>

#include <monitor/Monitor.h>
#include <policies/Acceptance.h>
#include <policies/Cooling.h>
#include <policies/MoveSelector.h>
#include <policies/Resource.h>
#include <sa/SA.h>
#include <salesman/Position.h>
#include <salesman/Tester.h>

using namespace sa::core;
using namespace sa::monitor;
using namespace sa::policies;
using namespace sa::sa;
using namespace sa::targets::salesman;

void print(const SA& sa) { std::cout << sa.toString() << "\n" << sa.monitor.globalMetrics << "\n-----\n"; }

int main(int argc, char** argv)
{
  std::string mode = 1 < argc ? argv[1] : "";
  auto positions = generateTestCases();

  if (mode == "profile") {
    auto pos = std::move(positions.back());
    SA sa(std::make_unique<Iteration>(100000), std::make_unique<Metropolis>(), std::make_unique<Linear>(),
          std::make_unique<KBest>(1), Monitor(MonitorLevel::Low));
    sa.anneal(pos);
    print(sa);
  } else if (mode == "mem_check") {
    auto pos = std::move(positions.back());
    SA sa(std::make_unique<Iteration>(100000), std::make_unique<Metropolis>(), std::make_unique<Linear>(),
          std::make_unique<KBest>(1), Monitor(MonitorLevel::Low));
    sa.anneal(pos);
    print(sa);
  } else {
    std::size_t idx = 0;
    for (const auto& position : positions) {
      SA sa1(std::make_unique<Iteration>(5000), std::make_unique<Metropolis>(), std::make_unique<Linear>(),
             std::make_unique<KBest>(1), Monitor(MonitorLevel::Low));
      SA sa2(std::make_unique<Iteration>(100000), std::make_unique<Metropolis>(), std::make_unique<Linear>(),
             std::make_unique<KBest>(1), Monitor(MonitorLevel::Low));
      SA sa3(std::make_unique<Iteration>(10000000), std::make_unique<Metropolis>(), std::make_unique<Linear>(),
             std::make_unique<KBest>(1), Monitor(MonitorLevel::Low));
      sa1.anneal(position);
      print(sa1);
      sa2.anneal(position);
      print(sa2);
      sa3.anneal(position);
      print(sa3);
      SA sa4(std::make_unique<Time>(5), std::make_unique<Metropolis>(), std::make_unique<Linear>(),
             std::make_unique<KBest>(1), Monitor(MonitorLevel::Low));
      sa4.anneal(position);
      print(sa4);
      SA sa5(std::make_unique<Time>(5), std::make_unique<Metropolis>(), std::make_unique<Quadratic>(),
             std::make_unique<KBest>(1), Monitor(MonitorLevel::Low));
      sa5.anneal(position);
      print(sa5);
      ++idx;
    }
  }
  return 0;
}
