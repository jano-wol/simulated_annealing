#ifndef SIMULATED_ANNEALING_CORE_IGENERATOR_H_
#define SIMULATED_ANNEALING_CORE_IGENERATOR_H_

#include <core/IPosition.h>

namespace sa::core
{
class IGenerator
{
public:
    virtual std::shared_ptr<core::IPosition> generatePosition(int idx) = 0;
    virtual ~IGenerator() = default;
};
}  // namespace sa::core

#endif  // SIMULATED_ANNEALING_CORE_IGENERATOR_H_
