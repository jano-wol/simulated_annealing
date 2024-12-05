#ifndef SIMULATED_ANNEALING_SA_MOVE_CANDIDATE_H_
#define SIMULATED_ANNEALING_SA_MOVE_CANDIDATE_H_

#include <sstream>
#include <string>
#include <vector>

#include <core/IMove.h>
#include <core/IPosition.h>

namespace sa::sa
{
class MoveCandidate
{
public:
  MoveCandidate(double delta_, double energyCandidate_, core::IMove::CPtr move_, core::IPosition::CPtr neighbour_)
      : delta(delta_), energyCandidate(energyCandidate_), move(std::move(move_)), neighbour(std::move(neighbour_))
  {}

  double delta;
  double energyCandidate;
  core::IMove::CPtr move;
  core::IPosition::CPtr neighbour;
};
}  // namespace sa::sa

#endif  // SIMULATED_ANNEALING_SA_MOVE_CANDIDATE_H_
