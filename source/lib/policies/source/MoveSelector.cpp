#include <policies/MoveSelector.h>

#include <sstream>

using namespace sa::core;
using namespace sa::policies;

IMove::CPtr KBest::selectMove(const IPosition::CPtr& position) const
{
  auto bestMove = position->generateMove();
  for (int i = 1; i < k; ++i) {
    auto move = position->generateMove();
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
