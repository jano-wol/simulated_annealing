#ifndef SIMULATED_ANNEALING_CORE_IGENERATOR_H_
#define SIMULATED_ANNEALING_CORE_IGENERATOR_H_

#include <core/IPosition.h>

namespace sa::core
{
class IGenerator
{
public:
  using CPtr = std::unique_ptr<IGenerator>;
  virtual core::IPosition::CPtr generatePosition(int idx) const = 0;
  virtual std::string getGeneratorName() const = 0;
  virtual ~IGenerator() = default;
};
}  // namespace sa::core

#endif  // SIMULATED_ANNEALING_CORE_IGENERATOR_H_
