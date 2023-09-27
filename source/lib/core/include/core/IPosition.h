#ifndef SIMULATED_ANNEALING_CORE_IPOSITION_H_
#define SIMULATED_ANNEALING_CORE_IPOSITION_H_

#include <core/IMove.h>
#include <memory>

namespace sa::core
{
class IPosition
{
public:
  virtual double getEnergy() = 0;
  virtual std::shared_ptr<IMove> getMove() = 0;
  virtual std::shared_ptr<IPosition> makeMove(std::shared_ptr<IMove>) = 0;
};
}  // namespace sa::core

#endif  // SIMULATED_ANNEALING_CORE_IPOSITION_H_
