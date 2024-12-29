#include <policies/Acceptance.h>

#include <cmath>
#include <sstream>

#include <core/Random.h>

using namespace sa::core;
using namespace sa::policies;

bool Metropolis::accept(double /*currEnergy*/, double delta, double temperature) const
{
  if (delta <= 0)
    return true;
  double threshold = std::exp(-delta / normalizator / temperature);
  double randomResult = r.randomDouble(0.0, 1.0);
  return randomResult < threshold;
}

std::string Metropolis::toString() const
{
  std::stringstream ss;
  ss << "Metropolis";
  if (normalizator != 1) {
    ss << "[norm=" << normalizator << "]";
  }
  return ss.str();
}

IAcceptance::CPtr Metropolis::clone() const { return std::make_unique<Metropolis>(normalizator); }

bool Greedy::accept(double /*currEnergy*/, double delta, double /*temperature*/) const { return delta < 0; }

std::string Greedy::toString() const { return "Greedy"; }

IAcceptance::CPtr Greedy::clone() const { return std::make_unique<Greedy>(); }
