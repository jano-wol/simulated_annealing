#ifndef SIMULATED_ANNEALING_TARGETS_SALESMAN_SALESMANPOSITION_H_
#define SIMULATED_ANNEALING_TARGETS_SALESMAN_SALESMANPOSITION_H_

#include <vector>

#include <core/IPosition.h>
#include <core/Random.h>
#include <salesman/Move.h>
#include <serializator/Serializator.h>

namespace sa::targets::salesman
{
class SalesmanPosition final : public core::IPosition
{
public:
  SalesmanPosition(std::vector<std::pair<double, double>> cities_) : cities(std::move(cities_))
  {
    energy = calcEnergy();
  }

  SalesmanPosition(double energy_, std::vector<std::pair<double, double>> cities_)
      : energy(energy_), cities(std::move(cities_))
  {}

  double getEnergy() const override;
  core::IMove::CPtr generateMove() const override;
  void makeMove(core::IMove::CPtr move) override;
  int size() const override;
  core::IPosition::CPtr clone() const override;
  void plot() const override;

  std::pair<std::size_t, std::size_t> getNeighbourIdxs(std::size_t idx) const;
  double distance(const std::pair<double, double>& city1, const std::pair<double, double>& city2) const;
  double calcEnergy() const;

  static std::string toString(const core::IPosition::CPtr& position);
  static core::IPosition::CPtr fromString(const std::string& data);

  double energy;
  std::vector<std::pair<double, double>> cities;
  core::Random r;

  static std::string getTypeIdStatic() { return "salesman"; }
  std::string getTypeId() const override { return getTypeIdStatic(); }
  inline static const bool registered = []() {
    serializator::Serializator::registerFromStringType(getTypeIdStatic(), fromString);
    serializator::Serializator::registerToStringType(getTypeIdStatic(), toString);
    return true;
  }();
};
}  // namespace sa::targets::salesman

#endif  // SIMULATED_ANNEALING_TARGETS_SALESMAN_SALESMANPOSITION_H_
