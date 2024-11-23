#ifndef SIMULATED_ANNEALING_SA_IPOSITION_H_
#define SIMULATED_ANNEALING_SA_IPOSITION_H_

#include <sa/IMove.h>
#include <memory>
#include <optional>

namespace sa::sa
{
class IPosition
{
public:
  virtual std::shared_ptr<IMove> getMove() const = 0;
  virtual double getEnergy() const = 0;
  virtual std::optional<double> getEnergyFast(const std::shared_ptr<IMove>& imove, double baseEnergy) const = 0;
  virtual std::shared_ptr<IPosition> makeMove(const std::shared_ptr<IMove>& imove) const = 0;
  virtual void makeMoveInplace(const std::shared_ptr<IMove>& imove) = 0;
};
}  // namespace sa::sa

#endif  // SIMULATED_ANNEALING_SA_IPOSITION_H_
