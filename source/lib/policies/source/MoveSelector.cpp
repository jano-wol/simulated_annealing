#include <policies/MoveSelector.h>

#include <sstream>

using namespace sa::core;
using namespace sa::policies;

IMove::CPtr KBest::selectMove(const IPosition::CPtr& position) const
{
  auto bestMove = position->generateMoveCandidate();
  for (int i = 1; i < k; ++i) {
    auto move = position->generateMoveCandidate();
    if (move->getDelta() < bestMove->getDelta()) {
      bestMove = std::move(move);
    }
  }
  return bestMove;
}

std::string KBest::toString() const
{
  std::stringstream ss;
  ss << "KBest";
  if (k != 1) {
    ss << "[k=" << k << "]";
  }
  return ss.str();
}

IMoveSelector::CPtr KBest::clone() const { return std::make_unique<KBest>(k); }
