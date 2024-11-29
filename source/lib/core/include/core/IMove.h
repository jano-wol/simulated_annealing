#ifndef SIMULATED_ANNEALING_CORE_IMOVE_H_
#define SIMULATED_ANNEALING_CORE_IMOVE_H_

#include <memory>

namespace sa::core
{
class IMove
{
public:
  using Ptr = std::shared_ptr<IMove>;
  virtual ~IMove() = default;
};
}  // namespace sa::core

#endif  // SIMULATED_ANNEALING_CORE_IMOVE_H_
