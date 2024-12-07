#ifndef SIMULATED_ANNEALING_CORE_IMOVE_H_
#define SIMULATED_ANNEALING_CORE_IMOVE_H_

#include <memory>

namespace sa::core
{
class IMove
{
public:
  using CPtr = std::unique_ptr<IMove>;
  virtual double getDelta() const = 0;
  virtual int size() const = 0;
  virtual ~IMove() = default;
};
}  // namespace sa::core

#endif  // SIMULATED_ANNEALING_CORE_IMOVE_H_
