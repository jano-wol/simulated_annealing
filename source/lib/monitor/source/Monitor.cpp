#include <monitor/Monitor.h>

#include <algorithm>
#include <sstream>

using namespace sa::monitor;

int GlobalMetrics::size() const { return sizeof(*this); }

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
    if (energies.getCount() == 1) {
      localDerivative = 0;
    } else {
      localDerivative = (energies.getNewest() - energies.getOldest()) / double(energies.getCount() - 1);
    }
    minEnergy = *minIt;
    maxEnergy = *maxIt;
  }
}

int Snapshot::size() const { return 3 * sizeof(double) + deltaStats.size() + globalMetrics.size() + position->size(); }

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
    if (level == MonitorLevel::Medium) {
      addSnapshotChecked(position, progress);
    }
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
  if (level == MonitorLevel::High && snapshotsMemory < snapshotsMemoryLimit) {
    addSnapshot(position);
  }
}

void Monitor::onEnd(const core::IPosition::CPtr& position)
{
  refreshGlobalMetrics();
  if (level > MonitorLevel::Low) {
    energies.push(position->getEnergy());
    if (level == MonitorLevel::Medium) {
      addSnapshot(position);
    }
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
  snapshotsMemory += snapshots.back().size();
}

void Monitor::addSnapshotChecked(const core::IPosition::CPtr& position, double progress)
{
  if (snapshots.size() < steps && snapshots.size() < progress * double(steps) &&
      snapshotsMemory < snapshotsMemoryLimit) {
    addSnapshot(position);
  }
}

std::string Monitor::toString() const
{
  std::stringstream ss;
  ss << "Monitor[level=" << static_cast<int>(level) << ";bestCatchQ=" << bestCatchQ
     << ";catchPrecision=" << catchPrecision;
  if (level > MonitorLevel::Low) {
    ss << ";localEnv=" << localEnv << ";steps=" << steps << ";snapshotsMemoryLimit=" << snapshotsMemoryLimit;
  }
  ss << "]";
  return ss.str();
}

std::ostream& sa::monitor::operator<<(std::ostream& os, const GlobalMetrics& gm)
{
  os << "GlobalMetrics["
     << "bestEnergy=" << gm.bestEnergy << ",bestIdx=" << gm.bestIdx << ",allIt=" << gm.idx
     << ",acceptedIt=" << gm.acceptance << ",upEnergyChanges=" << gm.upEnergyChanges << ",bestClone=" << gm.bestCatch
     << ",duration=" << gm.duration << "s,speed=" << int(gm.speed) << "/s"
     << "]";
  return os;
}
