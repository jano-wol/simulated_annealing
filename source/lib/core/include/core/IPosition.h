#ifndef SIMULATED_ANNEALING_CORE_IPOSITION_H_
#define SIMULATED_ANNEALING_CORE_IPOSITION_H_

#include <core/IMove.h>
#include <memory>
#include <optional>

namespace sa::core
{
class IPosition
{
public:
  virtual std::shared_ptr<IMove> getMove() = 0;
  virtual double getEnergy() = 0;
  virtual std::optional<double> getEnergyFast(const std::shared_ptr<IMove>& imove, double baseEnergy) = 0;
  virtual std::shared_ptr<IPosition> makeMove(const std::shared_ptr<IMove>& imove) = 0;
  virtual void makeMoveInplace(const std::shared_ptr<IMove>& imove) = 0;
};
}  // namespace sa::core

#endif  // SIMULATED_ANNEALING_CORE_IPOSITION_H_
