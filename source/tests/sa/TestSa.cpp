#include <gmock/gmock.h>

#include <cmath>
#include <memory>

#include <core/IPosition.h>
#include <core/Random.h>
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
  DummyFastMove2() { --d; }
  double getDelta() const override { return d; }
  int size() const override { return 0; }
  static int d;
};

class DummyFastMove3 : public IMove
{
public:
  DummyFastMove3() { d = Random::randomInt(-5, 5); }
  double getDelta() const override { return d; }
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
  IMove::CPtr generateMove() const override
  {
    if (mode == 0) {
      return std::make_unique<DummyFastMove>();
    } else if (mode == 1) {
      return std::make_unique<DummyFastMove2>();
    } else {
      return std::make_unique<DummyFastMove3>();
    }
  }
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
  static int mode;
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
  DummyFastPosition::mode = 0;
  DummyFastMove2::d = 0;
  DummyFastMove3::d = 0;
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
int DummyFastPosition::mode = 0;
int DummyFastMove2::d = 0;
int DummyFastMove3::d = 0;

void testSnapshot(const Snapshot& snapshot, double localDerivative, double minEnergy, double maxEnergy,
                  double deltaMean, double deltaDeviation)
{
  EXPECT_NEAR(snapshot.localDerivative, localDerivative, precision);
  EXPECT_NEAR(snapshot.globalMetrics.bestEnergy, snapshot.minEnergy, precision);
  EXPECT_NEAR(snapshot.minEnergy, minEnergy, precision);
  EXPECT_NEAR(snapshot.maxEnergy, maxEnergy, precision);
  EXPECT_TRUE(snapshot.deltaStats.mean.has_value());
  EXPECT_TRUE(snapshot.deltaStats.deviation.has_value());
  EXPECT_NEAR(*snapshot.deltaStats.mean, deltaMean, 1e-3);
  EXPECT_NEAR(*snapshot.deltaStats.deviation, deltaDeviation, 1e-3);
}
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
  EXPECT_TRUE(snapshot0.deltaStats.mean.has_value());
  EXPECT_TRUE(snapshot0.deltaStats.deviation.has_value());
  EXPECT_NEAR(*snapshot0.deltaStats.mean, -1, precision);
  EXPECT_NEAR(*snapshot0.deltaStats.deviation, 0, precision);
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

TEST(Sa, SnapshotCount1)
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

TEST(Sa, SnapshotCount2)
{
  for (int i = 1; i < 10; ++i) {
    SA sa(std::make_unique<Iteration>(100), std::make_unique<Metropolis>(), std::make_unique<Linear>(),
          std::make_unique<KBest>(1), Monitor(MonitorLevel::Medium));
    sa.monitor.steps = i;
    sa.monitor.snapshotsMemoryLimit = 0;
    IPosition::CPtr position = std::make_unique<DummyFastPosition>(0);
    sa.anneal(position);
    EXPECT_EQ(sa.monitor.snapshots.size(), 2);
    nullStatics();
  }
}

TEST(Sa, SnapshotCount3)
{
  for (int i = 1; i < 20; ++i) {
    SA sa(std::make_unique<Iteration>(1000), std::make_unique<Metropolis>(), std::make_unique<Linear>(),
          std::make_unique<KBest>(1), Monitor(MonitorLevel::Medium));
    sa.monitor.steps = i;
    sa.monitor.snapshotsMemoryLimit = 1000;
    IPosition::CPtr position = std::make_unique<DummyFastPosition>(0);
    sa.anneal(position);
    if (i < 9) {
      EXPECT_EQ(sa.monitor.snapshots.size(), sa.monitor.steps + 1);
    } else {
      EXPECT_EQ(sa.monitor.snapshots.size(), 9);
      EXPECT_EQ(sa.monitor.snapshotsMemory, 1152);
    }
    nullStatics();
  }
}

TEST(Sa, SnapshotCount4)
{
  SA sa(std::make_unique<Iteration>(1000), std::make_unique<Metropolis>(), std::make_unique<Linear>(),
        std::make_unique<KBest>(1), Monitor(MonitorLevel::High));
  IPosition::CPtr position = std::make_unique<DummyFastPosition>(0);
  sa.anneal(position);
  EXPECT_EQ(sa.monitor.snapshots.size(), sa.monitor.globalMetrics.acceptance + 1);
  nullStatics();
}

TEST(Sa, SnapshotCount5)
{
  for (std::size_t i = 100; i < 200; ++i) {
    DummyFastPosition::mode = 2;
    SA sa(std::make_unique<Iteration>(i), std::make_unique<Metropolis>(), std::make_unique<Linear>(),
          std::make_unique<KBest>(1), Monitor(MonitorLevel::High));
    IPosition::CPtr position = std::make_unique<DummyFastPosition>(0);
    sa.anneal(position);
    EXPECT_EQ(sa.monitor.snapshots.size(), sa.monitor.globalMetrics.acceptance + 1);
    EXPECT_TRUE(5 < sa.monitor.globalMetrics.acceptance && sa.monitor.globalMetrics.acceptance < i - 5);
    nullStatics();
  }
}

TEST(Sa, Statistics1)
{
  for (int t = 2; t < 20; ++t) {
    SA sa(std::make_unique<Iteration>(1000), std::make_unique<Metropolis>(), std::make_unique<Linear>(),
          std::make_unique<KBest>(1), Monitor(MonitorLevel::Medium, 0.9, 1e-6, t));
    IPosition::CPtr position = std::make_unique<DummyFastPosition>(0);
    sa.anneal(position);
    auto& snapshot0 = sa.monitor.snapshots[0];
    EXPECT_TRUE(snapshot0.deltaStats.mean.has_value());
    EXPECT_TRUE(snapshot0.deltaStats.deviation.has_value());
    EXPECT_NEAR(*snapshot0.deltaStats.mean, -1, precision);
    EXPECT_NEAR(*snapshot0.deltaStats.deviation, 0, precision);
    EXPECT_NEAR(snapshot0.localDerivative, 0, precision);
    EXPECT_NEAR(snapshot0.globalMetrics.bestEnergy, snapshot0.minEnergy, precision);
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
}

TEST(Sa, Statistics2)
{
  DummyFastPosition::mode = 1;
  SA sa(std::make_unique<Iteration>(1000), std::make_unique<Metropolis>(), std::make_unique<Linear>(),
        std::make_unique<KBest>(1), Monitor(MonitorLevel::Medium, 0.9, 1e-6, 10));
  IPosition::CPtr position = std::make_unique<DummyFastPosition>(0);
  sa.anneal(position);
  double dev10 = 2.87228;
  testSnapshot(sa.monitor.snapshots[0], 0, 0, 0, -1, 0);
  testSnapshot(sa.monitor.snapshots[1], -46, -1275, -861, -46.5, dev10);
  testSnapshot(sa.monitor.snapshots[2], -97, -5151, -4278, -97.5, dev10);
  testSnapshot(sa.monitor.snapshots[19], -946, -451725, -443211, -946.5, dev10);
  testSnapshot(sa.monitor.snapshots[20], -996, -500500, -491536, -995.5, dev10);
  nullStatics();
}

TEST(Sa, Statistics3)
{
  DummyFastPosition::mode = 1;
  SA sa(std::make_unique<Iteration>(1000), std::make_unique<Metropolis>(), std::make_unique<Linear>(),
        std::make_unique<KBest>(1), Monitor(MonitorLevel::Medium));
  IPosition::CPtr position = std::make_unique<DummyFastPosition>(1);
  sa.anneal(position);
  testSnapshot(sa.monitor.snapshots[0], 0, 1, 1, -1, 0);
  testSnapshot(sa.monitor.snapshots[1], -25.5, -1274, 1, -26, 14.7196);
  testSnapshot(sa.monitor.snapshots[2], -51, -5150, 1, -51.5, 29.4434);
  testSnapshot(sa.monitor.snapshots[19], -475.5, -451724, 1, -476, 274.5300);
  testSnapshot(sa.monitor.snapshots[20], -501, -500499, 0, -500.5, 288.6750);
  nullStatics();
}

TEST(Sa, SnapshotAlignment)
{
  std::vector<std::size_t> loc{10, 20, 33, 100, 101, 1000, 1001};
  for (auto l : loc) {
    DummyFastPosition::mode = 1;
    SA sa1(std::make_unique<Iteration>(1000), std::make_unique<Metropolis>(), std::make_unique<Linear>(),
           std::make_unique<KBest>(1), Monitor(MonitorLevel::Medium, 0.9, 1.0e-6, l));
    SA sa2(std::make_unique<Iteration>(1000), std::make_unique<Metropolis>(), std::make_unique<Linear>(),
           std::make_unique<KBest>(1), Monitor(MonitorLevel::High, 0.9, 1.0e-6, l));
    IPosition::CPtr position = std::make_unique<DummyFastPosition>(0);
    sa1.anneal(position);
    nullStatics();
    DummyFastPosition::mode = 1;
    sa2.anneal(position);
    for (const auto& s : sa1.monitor.snapshots) {
      auto idx = s.globalMetrics.idx;
      const auto& ss = sa2.monitor.snapshots[idx];
      EXPECT_NEAR(*s.deltaStats.mean, *ss.deltaStats.mean, 1e-3);
      EXPECT_NEAR(*s.deltaStats.deviation, *ss.deltaStats.deviation, 1e-3);
      EXPECT_NEAR(s.position->getEnergy(), ss.position->getEnergy(), 1e-3);
    }
    nullStatics();
  }
}
