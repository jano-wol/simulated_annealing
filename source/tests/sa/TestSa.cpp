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

class DummySlowPosition : public IPosition
{
public:
  DummySlowPosition(double energy_) : energy(energy_) { ++created; }
  DummySlowPosition(const DummySlowPosition& other) : energy(other.energy) { ++created; }
  DummySlowPosition(DummySlowPosition&& other) noexcept : energy(other.energy) { ++created; }

  DummySlowPosition& operator=(DummySlowPosition&& other) noexcept
  {
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
  static std::size_t created;
  static std::size_t getEnergyCounter;
  static std::size_t makeMoveCounter;
  static std::size_t cloneCounter;
};

class DummyFastPosition : public IPosition
{
public:
  DummyFastPosition(double energy_) : energy(energy_) { ++created; }
  DummyFastPosition(const DummyFastPosition& other) : energy(other.energy) { ++created; }
  DummyFastPosition(DummyFastPosition&& other) noexcept : energy(other.energy) { ++created; }

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
  static std::size_t created;
  static std::size_t getEnergyCounter;
  static std::size_t getDeltaCounter;
  static std::size_t makeMoveCounter;
  static std::size_t createNeighbourCounter;
  static std::size_t cloneCounter;
};

void nullStatics()
{
  DummySlowPosition::created = 0;
  DummySlowPosition::getEnergyCounter = 0;
  DummySlowPosition::makeMoveCounter = 0;
  DummySlowPosition::cloneCounter = 0;
  DummyFastPosition::created = 0;
  DummyFastPosition::getEnergyCounter = 0;
  DummyFastPosition::getDeltaCounter = 0;
  DummyFastPosition::makeMoveCounter = 0;
  DummyFastPosition::createNeighbourCounter = 0;
  DummyFastPosition::cloneCounter = 0;
}

std::size_t DummySlowPosition::created = 0;
std::size_t DummySlowPosition::getEnergyCounter = 0;
std::size_t DummySlowPosition::makeMoveCounter = 0;
std::size_t DummySlowPosition::cloneCounter = 0;
std::size_t DummyFastPosition::created = 0;
std::size_t DummyFastPosition::getEnergyCounter = 0;
std::size_t DummyFastPosition::getDeltaCounter = 0;
std::size_t DummyFastPosition::makeMoveCounter = 0;
std::size_t DummyFastPosition::createNeighbourCounter = 0;
std::size_t DummyFastPosition::cloneCounter = 0;
}  // namespace

TEST(Sa, SlowAnnealing)
{
  SA sa(std::make_unique<Iteration>(1000), std::make_unique<Metropolis>(), std::make_unique<Linear>(),
        std::make_unique<KBest>(1), Monitor(MonitorLevel::Low));
  IPosition::CPtr position = std::make_unique<DummySlowPosition>(0);
  sa.anneal(position);
  EXPECT_EQ(DummySlowPosition::created, 1002);
  EXPECT_EQ(DummySlowPosition::getEnergyCounter, 1000);
  EXPECT_EQ(DummySlowPosition::makeMoveCounter, 1000);
  EXPECT_EQ(DummySlowPosition::cloneCounter, 1);
  EXPECT_NEAR(sa.currPosition->getEnergy(), -1000, precision);
  nullStatics();
}

TEST(Sa, FastAnnealing)
{
  SA sa(std::make_unique<Iteration>(1000), std::make_unique<Metropolis>(), std::make_unique<Linear>(),
        std::make_unique<KBest>(1), Monitor(MonitorLevel::Low));
  IPosition::CPtr position = std::make_unique<DummyFastPosition>(0);
  sa.anneal(position);
  EXPECT_EQ(DummyFastPosition::created, 2);
  EXPECT_EQ(DummyFastPosition::getEnergyCounter, 1000);
  EXPECT_EQ(DummyFastPosition::makeMoveCounter, 1000);
  EXPECT_EQ(DummyFastPosition::cloneCounter, 1);
  EXPECT_NEAR(sa.currPosition->getEnergy(), -1000, precision);
  nullStatics();
}