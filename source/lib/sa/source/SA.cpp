#include <sa/SA.h>

#include <sstream>

using namespace sa::core;
using namespace sa::sa;

void SA::anneal(const IPosition::CPtr& startPosition)
{
  currPosition = startPosition->clone();
  while (resourcePolicy->getLeft() > 0) {
    auto move = moveSelectorPolicy->selectMove(currPosition);
    double progress = 1.0 - (resourcePolicy->getLeft() / resourcePolicy->getAll());
    double temperature = coolingPolicy->getTemperature(progress);
    monitor.candidatePhase();
    if (acceptancePolicy->accept(currPosition->getEnergy(), move->getDelta(), temperature)) {
      monitor.acceptancePhase();
      currPosition->makeMove(std::move(move));
    }
    resourcePolicy->updateLeft();
  }
}

std::string SA::toString() const
{
  std::stringstream ss;
  ss << "<" << resourcePolicy->toString() << ";" << acceptancePolicy->toString() << ";" << coolingPolicy->toString()
     << ";" << moveSelectorPolicy->toString() << ";" << monitor.toString() << ">";
  return ss.str();
}
