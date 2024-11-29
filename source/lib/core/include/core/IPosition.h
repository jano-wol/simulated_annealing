#ifndef SIMULATED_ANNEALING_CORE_IPOSITION_H_
#define SIMULATED_ANNEALING_CORE_IPOSITION_H_

#include <memory>
#include <optional>

#include <core/IMove.h>

namespace sa::core
{
class IPosition
{
public:
  using CPtr = std::unique_ptr<IPosition>;
  virtual IMove::Ptr generateMove() const = 0;
  virtual double getEnergy() const = 0;
  virtual std::optional<double> getDelta(const IMove::Ptr& imove) const = 0;
  virtual void makeMove(const IMove::Ptr& imove) = 0;
  virtual CPtr createNeighbour(const IMove::Ptr& imove) const = 0;
  virtual CPtr clone() const = 0;
  virtual ~IPosition() = default;
};
}  // namespace sa::core

#endif  // SIMULATED_ANNEALING_CORE_IPOSITION_H_
