#ifndef SIMULATED_ANNEALING_CORE_IMOVE_H_
#define SIMULATED_ANNEALING_CORE_IMOVE_H_

namespace sa::core
{
class IMove
{
public:
    virtual ~IMove() = default;
};
}  // namespace sa::core

#endif  // SIMULATED_ANNEALING_CORE_IMOVE_H_
