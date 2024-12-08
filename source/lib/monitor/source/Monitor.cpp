#include <monitor/Monitor.h>

#include <sstream>

using namespace sa::monitor;

void Monitor::onStart(const core::IPosition::CPtr& startPosition)
{
  startTime = std::chrono::high_resolution_clock::now();
  globalMetrics.bestEnergy = startPosition->getEnergy();
  bestCatch(startPosition, 0);
}

void Monitor::onCandidate(const core::IMove::CPtr& /*move*/, double /*delta*/) { ++globalMetrics.idx; }

void Monitor::onAcceptance(const core::IPosition::CPtr& position, double delta, double progress)
{
  ++globalMetrics.acceptance;
  if (delta > 0) {
    ++globalMetrics.upEnergyChanges;
  }
  double energy = position->getEnergy();
  if (energy < globalMetrics.bestEnergy || ((energy == globalMetrics.bestEnergy) && !bestPosition)) {
    globalMetrics.bestEnergy = energy;
    globalMetrics.bestIdx = globalMetrics.idx;
    bestCatch(position, progress);
  }
}

void Monitor::onEnd()
{
  auto elapsed = std::chrono::high_resolution_clock::now() - startTime;
  globalMetrics.duration = std::chrono::duration<double>(elapsed).count();
  globalMetrics.speed = double(globalMetrics.idx) / globalMetrics.duration;
}

void Monitor::bestCatch(const core::IPosition::CPtr& position, double progress)
{
  if (bestCatchQ < progress) {
    ++globalMetrics.bestCatch;
    bestPosition = position->clone();
  }
}

std::string Monitor::toString() const
{
  std::stringstream ss;
  ss << "Monitor[level=" << static_cast<int>(level) << ";localEnv=" << localEnv << ";bestCatchQ=" << bestCatchQ;
  return ss.str();
}

std::ostream& sa::monitor::operator<<(std::ostream& os, const GlobalMetrics& gm)
{
  os << "GlobalMetrics[" << "bestEnergy=" << gm.bestEnergy << ",bestIdx=" << gm.bestIdx << ",allIt=" << gm.idx
     << ",acceptedIt=" << gm.acceptance << ",upEnergyChanges=" << gm.upEnergyChanges << ",bestClone=" << gm.bestCatch
     << ",duration=" << gm.duration << "s, speed=" << gm.speed << "/s" << "]";
  return os;
}
