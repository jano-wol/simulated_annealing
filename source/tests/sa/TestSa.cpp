#include <gmock/gmock.h>

#include <cmath>
#include <memory>

#include <core/IPosition.h>
#include <policies/Acceptance.h>
#include <policies/Cooling.h>
#include <policies/Resource.h>
#include <sa/SA.h>

using namespace sa::core;
using namespace sa::policies;
using namespace sa::sa;

namespace
{
double delta = 1e-9;

class DummyMove : public IMove
{};

class DummySlowPosition : public IPosition
{
public:
  DummySlowPosition(double energy_) : energy(energy_) { ++created; }
  DummySlowPosition(const DummySlowPosition& other) : energy(other.energy) { ++created; }
  DummySlowPosition(DummySlowPosition&& other) noexcept : energy(other.energy) { ++created; }

  IMove::CPtr generateMove() const override { return std::make_unique<DummyMove>(); }
  double getEnergy() const override
  {
    ++getEnergyCounter;
    return energy;
  }
  std::optional<double> getDelta(const IMove::CPtr& /*imove*/) const override
  {
    ++getDeltaCounter;
    return std::nullopt;
  }
  void makeMove(const IMove::CPtr& /*imove*/) override { ++makeMoveCounter; }
  IPosition::CPtr createNeighbour(const IMove::CPtr& /*imove*/) const override
  {
    ++createNeighbourCounter;
    auto ret = std::make_unique<DummySlowPosition>(energy - 1);
    return ret;
  }
  IPosition::CPtr clone() const override
  {
    ++cloneCounter;
    auto ret = std::make_unique<DummySlowPosition>(energy);
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

class DummyFastPosition : public IPosition
{
public:
  DummyFastPosition(double energy_) : energy(energy_) { ++created; }
  DummyFastPosition(const DummyFastPosition& other) : energy(other.energy) { ++created; }
  DummyFastPosition(DummyFastPosition&& other) noexcept : energy(other.energy) { ++created; }

  IMove::CPtr generateMove() const override { return std::make_unique<DummyMove>(); }
  double getEnergy() const override
  {
    ++getEnergyCounter;
    return energy;
  }
  std::optional<double> getDelta(const IMove::CPtr& /*imove*/) const override
  {
    ++getDeltaCounter;
    return -1;
  }
  void makeMove(const IMove::CPtr& /*imove*/) override { ++makeMoveCounter; }
  IPosition::CPtr createNeighbour(const IMove::CPtr& /*imove*/) const override
  {
    ++createNeighbourCounter;
    auto ret = std::make_unique<DummyFastPosition>(energy - 1);
    return ret;
  }
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
  DummySlowPosition::getDeltaCounter = 0;
  DummySlowPosition::makeMoveCounter = 0;
  DummySlowPosition::createNeighbourCounter = 0;
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
std::size_t DummySlowPosition::getDeltaCounter = 0;
std::size_t DummySlowPosition::makeMoveCounter = 0;
std::size_t DummySlowPosition::createNeighbourCounter = 0;
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
  SA sa(std::make_unique<Iteration>(1000), std::make_unique<Metropolis>(), std::make_unique<Linear>());
  IPosition::CPtr position = std::make_unique<DummySlowPosition>(0);
  sa.anneal(position);
  EXPECT_EQ(DummySlowPosition::created, 1002);
  EXPECT_EQ(DummySlowPosition::getEnergyCounter, 1001);
  EXPECT_EQ(DummySlowPosition::getDeltaCounter, 1000);
  EXPECT_EQ(DummySlowPosition::makeMoveCounter, 0);
  EXPECT_EQ(DummySlowPosition::createNeighbourCounter, 1000);
  EXPECT_EQ(DummySlowPosition::cloneCounter, 1);
  EXPECT_NEAR(sa.bestEnergy, -1000, delta);
  nullStatics();
}

TEST(Sa, FastAnnealing)
{
  SA sa(std::make_unique<Iteration>(1000), std::make_unique<Metropolis>(), std::make_unique<Linear>());
  IPosition::CPtr position = std::make_unique<DummyFastPosition>(0);
  sa.anneal(position);
  EXPECT_EQ(DummyFastPosition::created, 2);
  EXPECT_EQ(DummyFastPosition::getEnergyCounter, 1);
  EXPECT_EQ(DummyFastPosition::getDeltaCounter, 1000);
  EXPECT_EQ(DummyFastPosition::makeMoveCounter, 1000);
  EXPECT_EQ(DummyFastPosition::createNeighbourCounter, 0);
  EXPECT_EQ(DummyFastPosition::cloneCounter, 1);
  EXPECT_NEAR(sa.bestEnergy, -1000, delta);
  nullStatics();
}