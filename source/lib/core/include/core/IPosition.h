#ifndef SIMULATED_ANNEALING_CORE_IPOSITION_H_
#define SIMULATED_ANNEALING_CORE_IPOSITION_H_

#include <memory>

#include <core/IMove.h>

namespace sa::core
{
class Candidate;

class IPosition
{
public:
  using CPtr = std::unique_ptr<IPosition>;
  virtual double getEnergy() const = 0;
  virtual IMove::CPtr generateMove() const = 0;
  virtual void makeMove(IMove::CPtr move) = 0;
  virtual int size() const = 0;
  virtual CPtr clone() const = 0;

  virtual void plot() const {}
  virtual ~IPosition() = default;
};
}  // namespace sa::core

#endif  // SIMULATED_ANNEALING_CORE_IPOSITION_H_
