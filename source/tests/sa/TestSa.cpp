#include <gmock/gmock.h>

#include <cmath>
#include <memory>

#include <core/IPosition.h>
#include <policies/Acceptance.h>
#include <policies/Cooling.h>
#include <policies/Resource.h>
#include <sa/sa.h>

using namespace sa::core;
using namespace sa::policies;
using namespace sa::sa;

namespace
{
class DummyMove : public IMove
{};

class DummySlowPosition : public IPosition
{
public:
  DummySlowPosition(double energy_) : energy(energy_) { ++created; }
  DummySlowPosition(const DummySlowPosition& other) : energy(other.energy) { ++created; }
  DummySlowPosition(DummySlowPosition&& other) noexcept : energy(other.energy) { ++created; }

  std::shared_ptr<IMove> generateMove() const override { return std::make_shared<DummyMove>(); }
  double getEnergy() const override
  {
    ++getEnergyCounter;
    return energy;
  }
  std::optional<double> getDelta(const std::shared_ptr<IMove>& /*imove*/) const override
  {
    ++getDeltaCounter;
    return std::nullopt;
  }
  void makeMove(const std::shared_ptr<IMove>& /*imove*/) override { ++makeMoveCounter; }
  std::shared_ptr<IPosition> createNeighbour(const std::shared_ptr<IMove>& /*imove*/) const override
  {
    ++createNeighbourCounter;
    auto ret = std::make_shared<DummySlowPosition>(energy - 1);
    return ret;
  }
  double energy;
  static std::size_t created;
  static std::size_t getEnergyCounter;
  static std::size_t getDeltaCounter;
  static std::size_t makeMoveCounter;
  static std::size_t createNeighbourCounter;
};

void nullStatics()
{
  DummySlowPosition::created = 0;
  DummySlowPosition::getEnergyCounter = 0;
  DummySlowPosition::getDeltaCounter = 0;
  DummySlowPosition::makeMoveCounter = 0;
  DummySlowPosition::createNeighbourCounter = 0;
}

std::size_t DummySlowPosition::created = 0;
std::size_t DummySlowPosition::getEnergyCounter = 0;
std::size_t DummySlowPosition::getDeltaCounter = 0;
std::size_t DummySlowPosition::makeMoveCounter = 0;
std::size_t DummySlowPosition::createNeighbourCounter = 0;
}  // namespace

TEST(Sa, SlowAnnealing)
{
  SA sa(Iteration(1000), Metropolis(), Linear());
  std::shared_ptr<IPosition> position = std::make_shared<DummySlowPosition>(0);
  sa.anneal(position);
  EXPECT_EQ(DummySlowPosition::created, 1001);
  EXPECT_EQ(DummySlowPosition::getEnergyCounter, 1001);
  EXPECT_EQ(DummySlowPosition::getDeltaCounter, 1000);
  EXPECT_EQ(DummySlowPosition::makeMoveCounter, 0);
  EXPECT_EQ(DummySlowPosition::createNeighbourCounter, 1000);
  EXPECT_NEAR(sa.bestEnergy, -1000, 1e-9);
  nullStatics();
}
