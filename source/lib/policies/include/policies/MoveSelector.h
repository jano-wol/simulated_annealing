#ifndef SIMULATED_ANNEALING_POLICIES_MOVE_SELECTOR_H_
#define SIMULATED_ANNEALING_POLICIES_MOVE_SELECTOR_H_

#include <string>

#include <core/MoveCandidate.h>

namespace
{
sa::core::MoveCandidate getMoveCandidate(const sa::core::IPosition::CPtr& position, double energy)
{
  double delta;
  double energyCandidate;
  auto m = position->generateMove();
  sa::core::IPosition::CPtr neighbour;

  auto deltaOpt = position->getDelta(m);
  if (!deltaOpt) {
    neighbour = position->createNeighbour(m);
    energyCandidate = neighbour->getEnergy();
    delta = energyCandidate - energy;
  } else {
    delta = *deltaOpt;
    energyCandidate = energy + delta;
  }
  return {delta, energyCandidate, std::move(m), std::move(neighbour)};
}

}  // namespace

namespace sa::policies
{
class IMoveSelector
{
public:
  using CPtr = std::unique_ptr<IMoveSelector>;
  virtual core::MoveCandidate selectMove(const core::IPosition::CPtr& position, double energy) const = 0;
  virtual std::string toString() const = 0;
  virtual ~IMoveSelector() = default;
};

class KBest final : public IMoveSelector
{
public:
  KBest(int k_) : k(k_) {}

  core::MoveCandidate selectMove(const core::IPosition::CPtr& position, double energy) const
  {
    core::MoveCandidate bestMoveCandidate = getMoveCandidate(position, energy);
    for (int i = 1; i < k; ++i) {
      auto moveCandidate = getMoveCandidate(position, energy);
      if (moveCandidate.delta < bestMoveCandidate.delta) {
        bestMoveCandidate = std::move(moveCandidate);
      }
    }
    return bestMoveCandidate;
  }

  std::string toString() const override
  {
    std::stringstream ss;
    ss << "KBest";
    if (k != 1) {
      ss << "[k=" << k << "]";
    }
    return ss.str();
  }

private:
  int k;
};
}  // namespace sa::policies

#endif  // SIMULATED_ANNEALING_POLICIES_MOVE_SELECTOR_H_