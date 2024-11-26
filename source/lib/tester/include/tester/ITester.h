#ifndef SIMULATED_ANNEALING_TESTER_ITESTER_H_
#define SIMULATED_ANNEALING_TESTER_ITESTER_H_

#include <core/IPosition.h>

namespace sa::tester
{
class ITester
{
public:
    virtual std::shared_ptr<core::IPosition> generateTestCase(int idx);
    virtual ~ITester() = default;
};
}  // namespace sa::tester

#endif  // SIMULATED_ANNEALING_TESTER_ITESTER_H_
