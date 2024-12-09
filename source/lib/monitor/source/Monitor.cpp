#include <monitor/Monitor.h>

#include <algorithm>
#include <sstream>

using namespace sa::monitor;

Snapshot::Snapshot(const core::IPosition::CPtr& position_, GlobalMetrics globalMetrics_,
                   const core::CircularBuffer& deltas, const core::CircularBuffer& energies)
    : deltaStats(deltas.getData()), globalMetrics(std::move(globalMetrics_)), position(position_->clone())
{
  if (energies.isEmpty()) {
    localDerivative = 0;
    minEnergy = globalMetrics.bestEnergy;
    maxEnergy = globalMetrics.bestEnergy;
  } else {
    auto sp = energies.getData();
    auto [minIt, maxIt] = std::minmax_element(sp.begin(), sp.end());
    localDerivative = (deltas.getNewest() - deltas.getOldest()) / double(deltas.getCount());
    minEnergy = *minIt;
    maxEnergy = *maxIt;
  }
}

void Monitor::onStart(const core::IPosition::CPtr& startPosition)
{
  startTime = std::chrono::high_resolution_clock::now();
  globalMetrics.bestEnergy = startPosition->getEnergy();
  bestCatch(startPosition, 0);
  if (level > MonitorLevel::Low) {
    addSnapshot(startPosition);
  }
}

void Monitor::onCandidate(const core::IPosition::CPtr& position, double delta, double energy, double progress)
{
  if (level > MonitorLevel::Low) {
    deltas.push(delta);
    energies.push(energy);
    addSnapshotChecked(position, progress);
    ++stalledAcceptance;
  }
  ++globalMetrics.idx;
}

void Monitor::onAcceptance(const core::IPosition::CPtr& position, double delta, double energy, double progress)
{
  ++globalMetrics.acceptance;
  if (delta > 0) {
    ++globalMetrics.upEnergyChanges;
  }
  if ((energy < globalMetrics.bestEnergy) ||
      (bestCatchQ < progress && !bestPosition && (energy - globalMetrics.bestEnergy < catchPrecision))) {
    globalMetrics.bestEnergy = energy;
    globalMetrics.bestIdx = globalMetrics.idx;
    bestCatch(position, progress);
  }
  if (level > MonitorLevel::Low) {
    stalledAcceptance = 0;
  }
}

void Monitor::onEnd(const core::IPosition::CPtr& position)
{
  refreshGlobalMetrics();
  if (level > MonitorLevel::Low) {
    addSnapshot(position);
  }
}

void Monitor::refreshGlobalMetrics()
{
  auto elapsed = std::chrono::high_resolution_clock::now() - startTime;
  globalMetrics.duration = std::chrono::duration<double>(elapsed).count();
  globalMetrics.speed = double(globalMetrics.idx) / globalMetrics.duration;
}

void Monitor::bestCatch(const core::IPosition::CPtr& position, double progress)
{
  if (bestCatchQ <= progress) {
    ++globalMetrics.bestCatch;
    bestPosition = position->clone();
  }
}

void Monitor::addSnapshot(const core::IPosition::CPtr& position)
{
  refreshGlobalMetrics();
  snapshots.push_back({position, globalMetrics, deltas, energies});
}

void Monitor::addSnapshotChecked(const core::IPosition::CPtr& position, double progress)
{
  if (0 < progress && snapshots.size() + 1 < steps && snapshots.size() < progress * double(steps)) {
    addSnapshot(position);
  }
}

std::string Monitor::toString() const
{
  std::stringstream ss;
  ss << "Monitor[level=" << static_cast<int>(level) << ";localEnv=" << localEnv << ";bestCatchQ=" << bestCatchQ
     << ";catchPrecision=" << catchPrecision << "]";
  return ss.str();
}

std::ostream& sa::monitor::operator<<(std::ostream& os, const GlobalMetrics& gm)
{
  os << "GlobalMetrics[" << "bestEnergy=" << gm.bestEnergy << ",bestIdx=" << gm.bestIdx << ",allIt=" << gm.idx
     << ",acceptedIt=" << gm.acceptance << ",upEnergyChanges=" << gm.upEnergyChanges << ",bestClone=" << gm.bestCatch
     << ",duration=" << gm.duration << "s,speed=" << int(gm.speed) << "/s" << "]";
  return os;
}
