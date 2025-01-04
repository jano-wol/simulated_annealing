#ifndef SIMULATED_ANNEALING_TARGETS_SALESMAN_ANGLE_GENERATOR_H_
#define SIMULATED_ANNEALING_TARGETS_SALESMAN_ANGLE_GENERATOR_H_

#include <core/IGenerator.h>

namespace sa::targets::salesman_angle
{
class SalesmanAngleGenerator final : public core::IGenerator
{
public:
  SalesmanAngleGenerator(int numCities_, int length_, bool subSquares_)
      : numCities(numCities_), length(length_), subSquares(subSquares_)
  {}

  core::IPosition::CPtr generatePosition(int idx) const override;
  std::string getGeneratorName() const override;

private:
  core::IPosition::CPtr getStartPosition(int idx) const;
  core::IPosition::CPtr getStartPosition_5x5(int idx) const;

  int numCities;
  int length;
  bool subSquares;
};
}  // namespace sa::targets::salesman_angle

#endif  // SIMULATED_ANNEALING_TARGETS_SALESMAN_ANGLE_GENERATOR_H_
