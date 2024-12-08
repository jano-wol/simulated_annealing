#include <monitor/Monitor.h>

using namespace sa::monitor;

void Monitor::onStart(const core::IPosition::CPtr& startPosition)
{
  startTime = std::chrono::high_resolution_clock::now();
  bestEnergy = startPosition->getEnergy();
  bestCatch(startPosition, 0);
}

void Monitor::onCandidate(const core::IMove::CPtr& move, double /*delta*/)
{
  ++idx;
  ++stalledAcceptance;
}

void Monitor::onAcceptance(const core::IPosition::CPtr& position, double delta, double progress)
{
  ++acceptance;
  if (delta > 0) {
    ++upEnergyChanges;
  }
  stalledAcceptance = 0;
  double energy = position->getEnergy();
  if (energy < bestEnergy) {
    bestEnergy = energy;
    bestIdx = idx;
    bestCatch(position, progress);
  }
}

void Monitor::onEnd()
{
  auto elapsed = std::chrono::high_resolution_clock::now() - startTime;
  double elapsedSeconds = std::chrono::duration<double>(elapsed).count();
  speed = double(idx) / elapsedSeconds;
}

void Monitor::bestCatch(const core::IPosition::CPtr& position, double progress)
{
  if (bestCatchQ < progress) {
    bestPosition = position->clone();
  }
}

std::string Monitor::toString() const { return ""; }
