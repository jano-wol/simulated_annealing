#ifndef SIMULATED_ANNEALING_POLICIES_MOVE_SELECTOR_H_
#define SIMULATED_ANNEALING_POLICIES_MOVE_SELECTOR_H_

#include <string>

#include <core/IPosition.h>

namespace sa::policies
{
class IMoveSelector
{
public:
  using CPtr = std::unique_ptr<IMoveSelector>;
  virtual core::IMove::CPtr selectMove(const core::IPosition::CPtr& position) const = 0;
  virtual std::string toString() const = 0;
  virtual ~IMoveSelector() = default;
};

class KBest final : public IMoveSelector
{
public:
  KBest(int k_) : k(k_) {}

  core::IMove::CPtr selectMove(const core::IPosition::CPtr& position) const override;
  std::string toString() const override;

private:
  int k;
};
}  // namespace sa::policies

#endif  // SIMULATED_ANNEALING_POLICIES_MOVE_SELECTOR_H_