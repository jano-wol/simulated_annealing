#ifndef SIMULATED_ANNEALING_TARGETS_SALESMAN_GENERATOR_H_
#define SIMULATED_ANNEALING_TARGETS_SALESMAN_GENERATOR_H_

#include <core/IGenerator.h>

namespace sa::targets::salesman
{
class SalesmanGenerator final : public core::IGenerator
{
public:
  SalesmanGenerator(int numCities_, int length_, bool subSquares_)
      : numCities(numCities_), length(length_), subSquares(subSquares_)
  {}

  core::IPosition::CPtr generatePosition(int idx) override;
  std::string getGeneratorName() const override;

private:
  core::IPosition::CPtr getStartPosition(int idx);
  core::IPosition::CPtr getStartPosition_5x5(int idx);

  int numCities;
  int length;
  bool subSquares;
};
}  // namespace sa::targets::salesman

#endif  // SIMULATED_ANNEALING_TARGETS_GENERATOR_TESTER_H_
