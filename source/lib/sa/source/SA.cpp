#include <sa/SA.h>

#include <sstream>

using namespace sa::core;
using namespace sa::sa;

void SA::anneal(const IPosition::CPtr& startPosition)
{
  resourcePolicy->onStart();
  monitor->onStart(startPosition);
  currPosition = startPosition->clone();
  while (resourcePolicy->getLeft() > 0) {
    auto move = moveSelectorPolicy->selectMove(currPosition);
    double progress = 1.0 - (resourcePolicy->getLeft() / resourcePolicy->getAll());
    double temperature = coolingPolicy->getTemperature(progress);
    double delta = move->getDelta();
    double energy = currPosition->getEnergy();
    monitor->onCandidate(currPosition, delta, energy, progress);
    if (acceptancePolicy->accept(energy, delta, temperature)) {
      currPosition->makeMove(std::move(move));
      monitor->onAcceptance(currPosition, delta, delta + energy, progress);
    }
    resourcePolicy->updateLeft();
  }
  monitor->onEnd(currPosition);
}

const IPosition::CPtr& SA::getBest() { return monitor->bestPosition ? monitor->bestPosition : currPosition; }

std::string SA::toString() const
{
  std::stringstream ss;
  ss << "<" << resourcePolicy->toString() << ";" << acceptancePolicy->toString() << ";" << coolingPolicy->toString()
     << ";" << moveSelectorPolicy->toString() << ";" << monitor->toString() << ">";
  return ss.str();
}
