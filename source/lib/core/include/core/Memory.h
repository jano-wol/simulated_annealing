#ifndef SIMULATED_ANNEALING_CORE_MEMORY_H_
#define SIMULATED_ANNEALING_CORE_MEMORY_H_

#include <memory>

namespace sa::core
{
class Memory
{
public:
  template <typename Target, typename Base>
  static std::unique_ptr<Target> cast(std::unique_ptr<Base>&& basePtr)
  {
    Target* rawPtr = dynamic_cast<Target*>(basePtr.get());
    return std::make_unique<Target>(*rawPtr);
  }
};
}  // namespace sa::core

#endif  // SIMULATED_ANNEALING_CORE_MEMORY_H_
