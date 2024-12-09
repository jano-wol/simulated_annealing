#include <gmock/gmock.h>

#include <cmath>
#include <memory>

#include <core/IPosition.h>
#include <monitor/Monitor.h>
#include <policies/Acceptance.h>
#include <policies/Cooling.h>
#include <policies/Resource.h>
#include <sa/SA.h>

using namespace sa::core;
using namespace sa::monitor;
using namespace sa::policies;
using namespace sa::sa;

namespace
{
double precision = 1e-9;

class DummySlowPosition;

class DummySlowMove : public IMove
{
public:
  DummySlowMove(std::unique_ptr<DummySlowPosition> candidate_, double delta_)
      : candidate(std::move(candidate_)), delta(delta_)
  {}

  double getDelta() const override { return delta; }
  int size() const override { return 0; }

  std::unique_ptr<DummySlowPosition> candidate;
  double delta;
};

class DummyFastMove : public IMove
{
  double getDelta() const override { return -1; }
  int size() const override { return 0; }
};

class DummyFastMove2 : public IMove
{
public:
  double getDelta() const override { return d--; }
  int size() const override { return 0; }
  static int d;
};

class DummySlowPosition : public IPosition
{
public:
  DummySlowPosition(double energy_) : energy(energy_) { ++energyConstructorCounter; }
  DummySlowPosition(const DummySlowPosition& other) : energy(other.energy) { ++copyConstructorCounter; }
  DummySlowPosition(DummySlowPosition&& other) noexcept : energy(other.energy) { ++moveConstructorCounter; }
  DummySlowPosition& operator=(const DummySlowPosition& other)
  {
    ++copyAssignmentCounter;
    if (this != &other) {
      energy = other.energy;
    }
    return *this;
  }
  DummySlowPosition& operator=(DummySlowPosition&& other) noexcept
  {
    ++moveAssignmentCounter;
    if (this != &other) {
      energy = std::move(other.energy);
    }
    return *this;
  }

  double getEnergy() const override
  {
    ++getEnergyCounter;
    return energy;
  }

  IMove::CPtr generateMove() const override
  {
    auto candidate = std::make_unique<DummySlowPosition>(*this);
    --candidate->energy;
    double delta = candidate->energy - this->energy;
    return std::make_unique<DummySlowMove>(std::move(candidate), delta);
  }

  void makeMove(IMove::CPtr move) override
  {
    auto* m = dynamic_cast<DummySlowMove*>(move.get());
    *this = std::move(*m->candidate);
    ++makeMoveCounter;
  }

  int size() const override { return sizeof(double); }

  IPosition::CPtr clone() const override
  {
    ++cloneCounter;
    return std::make_unique<DummySlowPosition>(energy);
  }

  double energy;
  static std::size_t energyConstructorCounter;
  static std::size_t copyConstructorCounter;
  static std::size_t moveConstructorCounter;
  static std::size_t copyAssignmentCounter;
  static std::size_t moveAssignmentCounter;
  static std::size_t getEnergyCounter;
  static std::size_t generateMoveCounter;
  static std::size_t makeMoveCounter;
  static std::size_t cloneCounter;
};

class DummyFastPosition : public IPosition
{
public:
  DummyFastPosition(double energy_) : energy(energy_) { ++energyConstructorCounter; }
  DummyFastPosition(const DummyFastPosition& other) : energy(other.energy) { ++copyConstructorCounter; }
  DummyFastPosition(DummyFastPosition&& other) noexcept : energy(other.energy) { ++moveConstructorCounter; }
  DummyFastPosition& operator=(const DummyFastPosition& other)
  {
    ++copyAssignmentCounter;
    if (this != &other) {
      energy = other.energy;
    }
    return *this;
  }
  DummyFastPosition& operator=(DummyFastPosition&& other) noexcept
  {
    ++moveAssignmentCounter;
    if (this != &other) {
      energy = std::move(other.energy);
    }
    return *this;
  }

  double getEnergy() const override
  {
    ++getEnergyCounter;
    return energy;
  }
  IMove::CPtr generateMove() const override { return std::make_unique<DummyFastMove>(); }
  void makeMove(IMove::CPtr move) override
  {
    energy += move->getDelta();
    ++makeMoveCounter;
  }
  int size() const override { return sizeof(double); }
  IPosition::CPtr clone() const override
  {
    ++cloneCounter;
    auto ret = std::make_unique<DummyFastPosition>(energy);
    return ret;
  }

  double energy;
  static std::size_t energyConstructorCounter;
  static std::size_t copyConstructorCounter;
  static std::size_t moveConstructorCounter;
  static std::size_t copyAssignmentCounter;
  static std::size_t moveAssignmentCounter;
  static std::size_t getEnergyCounter;
  static std::size_t generateMoveCounter;
  static std::size_t makeMoveCounter;
  static std::size_t cloneCounter;
};

void nullStatics()
{
  DummySlowPosition::energyConstructorCounter = 0;
  DummySlowPosition::copyConstructorCounter = 0;
  DummySlowPosition::moveConstructorCounter = 0;
  DummySlowPosition::copyAssignmentCounter = 0;
  DummySlowPosition::moveAssignmentCounter = 0;
  DummySlowPosition::getEnergyCounter = 0;
  DummySlowPosition::generateMoveCounter = 0;
  DummySlowPosition::makeMoveCounter = 0;
  DummySlowPosition::cloneCounter = 0;
  DummyFastPosition::energyConstructorCounter = 0;
  DummyFastPosition::copyConstructorCounter = 0;
  DummyFastPosition::moveConstructorCounter = 0;
  DummyFastPosition::copyAssignmentCounter = 0;
  DummyFastPosition::moveAssignmentCounter = 0;
  DummyFastPosition::getEnergyCounter = 0;
  DummyFastPosition::generateMoveCounter = 0;
  DummyFastPosition::makeMoveCounter = 0;
  DummyFastPosition::cloneCounter = 0;
  DummyFastMove2::d = 0;
}

std::size_t DummySlowPosition::energyConstructorCounter = 0;
std::size_t DummySlowPosition::copyConstructorCounter = 0;
std::size_t DummySlowPosition::moveConstructorCounter = 0;
std::size_t DummySlowPosition::copyAssignmentCounter = 0;
std::size_t DummySlowPosition::moveAssignmentCounter = 0;
std::size_t DummySlowPosition::getEnergyCounter = 0;
std::size_t DummySlowPosition::generateMoveCounter = 0;
std::size_t DummySlowPosition::makeMoveCounter = 0;
std::size_t DummySlowPosition::cloneCounter = 0;
std::size_t DummyFastPosition::energyConstructorCounter = 0;
std::size_t DummyFastPosition::copyConstructorCounter = 0;
std::size_t DummyFastPosition::moveConstructorCounter = 0;
std::size_t DummyFastPosition::copyAssignmentCounter = 0;
std::size_t DummyFastPosition::moveAssignmentCounter = 0;
std::size_t DummyFastPosition::getEnergyCounter = 0;
std::size_t DummyFastPosition::generateMoveCounter = 0;
std::size_t DummyFastPosition::makeMoveCounter = 0;
std::size_t DummyFastPosition::cloneCounter = 0;
int DummyFastMove2::d = 0;
}  // namespace

TEST(Sa, SlowAnnealing)
{
  SA sa(std::make_unique<Iteration>(1000), std::make_unique<Metropolis>(), std::make_unique<Linear>(),
        std::make_unique<KBest>(1), Monitor(MonitorLevel::Low, 1.1));
  IPosition::CPtr position = std::make_unique<DummySlowPosition>(0);
  sa.anneal(position);
  EXPECT_EQ(DummySlowPosition::energyConstructorCounter, 2);
  EXPECT_EQ(DummySlowPosition::copyConstructorCounter, 1000);
  EXPECT_EQ(DummySlowPosition::copyAssignmentCounter, 0);
  EXPECT_EQ(DummySlowPosition::moveConstructorCounter, 0);
  EXPECT_EQ(DummySlowPosition::moveAssignmentCounter, 1000);
  EXPECT_EQ(DummySlowPosition::getEnergyCounter, 1001);
  EXPECT_EQ(DummySlowPosition::makeMoveCounter, 1000);
  EXPECT_EQ(DummySlowPosition::cloneCounter, 1);
  EXPECT_NEAR(sa.currPosition->getEnergy(), -1000, precision);
  nullStatics();
}

TEST(Sa, FastAnnealing)
{
  SA sa(std::make_unique<Iteration>(1000), std::make_unique<Metropolis>(), std::make_unique<Linear>(),
        std::make_unique<KBest>(1), Monitor(MonitorLevel::Low, 1.1));
  IPosition::CPtr position = std::make_unique<DummyFastPosition>(0);
  sa.anneal(position);
  EXPECT_EQ(DummyFastPosition::energyConstructorCounter, 2);
  EXPECT_EQ(DummyFastPosition::copyConstructorCounter, 0);
  EXPECT_EQ(DummyFastPosition::copyAssignmentCounter, 0);
  EXPECT_EQ(DummyFastPosition::moveConstructorCounter, 0);
  EXPECT_EQ(DummyFastPosition::moveAssignmentCounter, 0);
  EXPECT_EQ(DummyFastPosition::getEnergyCounter, 1001);
  EXPECT_EQ(DummyFastPosition::makeMoveCounter, 1000);
  EXPECT_EQ(DummyFastPosition::cloneCounter, 1);
  EXPECT_NEAR(sa.currPosition->getEnergy(), -1000, precision);
  nullStatics();
}

TEST(Sa, FastAnnealingMonitorLow)
{
  SA sa(std::make_unique<Iteration>(1000), std::make_unique<Metropolis>(), std::make_unique<Linear>(),
        std::make_unique<KBest>(1), Monitor(MonitorLevel::Low));
  IPosition::CPtr position = std::make_unique<DummyFastPosition>(0);
  sa.anneal(position);
  EXPECT_EQ(DummyFastPosition::energyConstructorCounter, 102);
  EXPECT_EQ(DummyFastPosition::copyConstructorCounter, 0);
  EXPECT_EQ(DummyFastPosition::copyAssignmentCounter, 0);
  EXPECT_EQ(DummyFastPosition::moveConstructorCounter, 0);
  EXPECT_EQ(DummyFastPosition::moveAssignmentCounter, 0);
  EXPECT_EQ(DummyFastPosition::getEnergyCounter, 1001);
  EXPECT_EQ(DummyFastPosition::makeMoveCounter, 1000);
  EXPECT_EQ(DummyFastPosition::cloneCounter, 101);
  EXPECT_EQ(sa.monitor.globalMetrics.acceptance, 1000);
  EXPECT_EQ(sa.monitor.globalMetrics.idx, 1000);
  EXPECT_EQ(sa.monitor.globalMetrics.bestIdx, 1000);
  EXPECT_EQ(sa.monitor.globalMetrics.upEnergyChanges, 0);
  EXPECT_EQ(sa.monitor.globalMetrics.bestEnergy, -1000);
  EXPECT_NEAR(sa.currPosition->getEnergy(), -1000, precision);
  nullStatics();
}

TEST(Sa, FastAnnealingMonitorMedium)
{
  SA sa(std::make_unique<Iteration>(1000), std::make_unique<Metropolis>(), std::make_unique<Linear>(),
        std::make_unique<KBest>(1), Monitor(MonitorLevel::Medium));
  IPosition::CPtr position = std::make_unique<DummyFastPosition>(0);
  sa.anneal(position);
  EXPECT_EQ(DummyFastPosition::energyConstructorCounter, 123);
  EXPECT_EQ(DummyFastPosition::copyConstructorCounter, 0);
  EXPECT_EQ(DummyFastPosition::copyAssignmentCounter, 0);
  EXPECT_EQ(DummyFastPosition::moveConstructorCounter, 0);
  EXPECT_EQ(DummyFastPosition::moveAssignmentCounter, 0);
  EXPECT_EQ(DummyFastPosition::getEnergyCounter, 1002);
  EXPECT_EQ(DummyFastPosition::makeMoveCounter, 1000);
  EXPECT_EQ(DummyFastPosition::cloneCounter, 122);
  EXPECT_EQ(sa.monitor.globalMetrics.acceptance, 1000);
  EXPECT_EQ(sa.monitor.globalMetrics.idx, 1000);
  EXPECT_EQ(sa.monitor.globalMetrics.bestIdx, 1000);
  EXPECT_EQ(sa.monitor.globalMetrics.upEnergyChanges, 0);
  EXPECT_EQ(sa.monitor.globalMetrics.bestEnergy, -1000);
  EXPECT_NEAR(sa.currPosition->getEnergy(), -1000, precision);
  auto& snapshot0 = sa.monitor.snapshots[0];
  EXPECT_FALSE(snapshot0.deltaStats.mean.has_value());
  EXPECT_FALSE(snapshot0.deltaStats.deviation.has_value());
  EXPECT_NEAR(snapshot0.localDerivative, 0, precision);
  std::vector<int> toTest{1, 2, 3, 17, 18, 19, 20};
  for (auto idx : toTest) {
    auto& snapshot = sa.monitor.snapshots[idx];
    EXPECT_TRUE(snapshot.deltaStats.mean.has_value());
    EXPECT_TRUE(snapshot.deltaStats.deviation.has_value());
    EXPECT_NEAR(*snapshot.deltaStats.mean, -1, precision);
    EXPECT_NEAR(*snapshot.deltaStats.deviation, 0, precision);
    EXPECT_NEAR(snapshot.localDerivative, -1, precision);
    EXPECT_NEAR(snapshot.globalMetrics.bestEnergy, snapshot.minEnergy, precision);
  }
  nullStatics();
}

TEST(Sa, SnapshotCount)
{
  for (int i = 1; i < 100; ++i) {
    SA sa(std::make_unique<Iteration>(100), std::make_unique<Metropolis>(), std::make_unique<Linear>(),
          std::make_unique<KBest>(1), Monitor(MonitorLevel::Medium));
    sa.monitor.steps = i;
    IPosition::CPtr position = std::make_unique<DummyFastPosition>(0);
    sa.anneal(position);
    EXPECT_EQ(sa.monitor.snapshots.size(), i + 1);
    nullStatics();
    int gap = sa.monitor.snapshots[1].globalMetrics.idx - sa.monitor.snapshots[0].globalMetrics.idx;
    for (std::size_t j = 1; j < sa.monitor.snapshots.size(); ++j) {
      int currGap = sa.monitor.snapshots[j].globalMetrics.idx - sa.monitor.snapshots[j - 1].globalMetrics.idx;
      EXPECT_TRUE(std::abs(currGap - gap) < 3.0);
    }
  }
}