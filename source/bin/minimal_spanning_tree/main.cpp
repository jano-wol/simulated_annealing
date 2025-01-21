#include <iostream>

#include <io/Io.h>
#include <minimal_spanning_tree/Generator.h>
#include <monitor/Monitor.h>
#include <policies/Acceptance.h>
#include <policies/Cooling.h>
#include <policies/MoveSelector.h>
#include <policies/Resource.h>
#include <sa/SA.h>

using namespace sa::core;
using namespace sa::io;
using namespace sa::monitor;
using namespace sa::policies;
using namespace sa::sa;
using namespace sa::targets::minimal_spanning_tree;

void print(const SA& sa, std::size_t idx)
{
  std::cout << "idx=" << idx << "\n"
            << sa.toString() << "\n"
            << sa.monitor->globalMetrics << "\ncurrEnergy=" << sa.currPosition->getEnergy() << "\n----\n";
}

int main(int argc, char** argv)
{
  std::string mode = 1 < argc ? argv[1] : "";
  if (mode == "profile") {
    IGenerator::CPtr g = std::make_unique<MinimalSpanningTreeGenerator>(1000, 100, true);
    auto pos = Io::getPosition(g, 1);
    SA sa(std::make_unique<Iteration>(1000000), std::make_unique<Metropolis>(), std::make_unique<Linear>(),
          std::make_unique<KBest>(1), std::make_unique<Monitor>(MonitorLevel::Low));
    sa.anneal(pos);
    print(sa, 1);
  } else if (mode == "mem_check") {
    IGenerator::CPtr g = std::make_unique<MinimalSpanningTreeGenerator>(1000, 100, true);
    auto pos = Io::getPosition(g, 1);
    SA sa(std::make_unique<Iteration>(100000), std::make_unique<Metropolis>(), std::make_unique<Linear>(),
          std::make_unique<KBest>(1), std::make_unique<Monitor>(MonitorLevel::Medium));
    sa.anneal(pos);
    print(sa, 1);
  } else {
    std::vector<int> nc{5, 10, 20, 50, 100, 200, 500, 1000};
    std::vector<double> l{1.0, 2.0, 5.0, 10.0, 50.0, 100.0, 100.0, 100.0};
    for (std::size_t idx = 0; idx < 8; ++idx) {
      for (int b = 0; b < 2; ++b) {
        IGenerator::CPtr g = std::make_unique<MinimalSpanningTreeGenerator>(nc[idx], int(l[idx]), bool(b));
        auto position = Io::getPosition(g, 1);
        if (mode == "save") {
          Io::savePosition(g, 1);
        }
        SA sa1(std::make_unique<Iteration>(5000), std::make_unique<Metropolis>(), std::make_unique<Linear>(),
               std::make_unique<KBest>(1), std::make_unique<Monitor>(MonitorLevel::Low));
        SA sa2(std::make_unique<Iteration>(100000), std::make_unique<Metropolis>(), std::make_unique<Linear>(),
               std::make_unique<KBest>(1), std::make_unique<Monitor>(MonitorLevel::Low));
        SA sa3(std::make_unique<Iteration>(10000000), std::make_unique<Metropolis>(), std::make_unique<Linear>(),
               std::make_unique<KBest>(1), std::make_unique<Monitor>(MonitorLevel::Low));
        SA sa4(std::make_unique<Time>(5), std::make_unique<Metropolis>(), std::make_unique<Linear>(),
               std::make_unique<KBest>(1), std::make_unique<Monitor>(MonitorLevel::Low));
        SA sa5(std::make_unique<Time>(5), std::make_unique<Metropolis>(), std::make_unique<Quadratic>(),
               std::make_unique<KBest>(1), std::make_unique<Monitor>(MonitorLevel::Low));
        sa1.anneal(position);
        print(sa1, idx);
        sa2.anneal(position);
        print(sa2, idx);
        sa3.anneal(position);
        print(sa3, idx);
        sa4.anneal(position);
        print(sa4, idx);
        sa5.anneal(position);
        print(sa5, idx);
        Io::tryImproveBest(g, 1, sa1.getBest());
        Io::tryImproveBest(g, 1, sa2.getBest());
        Io::tryImproveBest(g, 1, sa3.getBest());
        Io::tryImproveBest(g, 1, sa4.getBest());
        Io::tryImproveBest(g, 1, sa5.getBest());
      }
    }
    Io::refreshBest(Io::getTargetPath("minimal_spanning_tree"));
  }
  return 0;
}
