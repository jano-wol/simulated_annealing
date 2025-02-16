#include <monitor/Monitor.h>

#include <algorithm>
#include <sstream>

using namespace sa::core;
using namespace sa::monitor;

int GlobalMetrics::size() const { return sizeof(*this); }

LocalStats::LocalStats(const CircularBuffer& deltas, const CircularBuffer& energies, double bestEnergy)
    : deltaStats(deltas.getData())
{
  if (energies.isEmpty()) {
    localDerivative = 0;
    minEnergy = bestEnergy;
    maxEnergy = bestEnergy;
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

int LocalStats::size() const { return 3 * sizeof(double) + deltaStats.size(); }

Snapshot::Snapshot(const IPosition::CPtr& position_, GlobalMetrics globalMetrics_,
                   const CircularBuffer& deltasCandidate, const CircularBuffer& deltasAcceptance,
                   const CircularBuffer& energiesCandidate, const CircularBuffer& energiesAcceptance)
    : candidate(deltasCandidate, energiesCandidate, globalMetrics_.bestEnergy),
      acceptance(deltasAcceptance, energiesAcceptance, globalMetrics_.bestEnergy),
      globalMetrics(std::move(globalMetrics_)),
      position(position_->clone())
{}

int Snapshot::size() const { return candidate.size() + acceptance.size() + globalMetrics.size() + position->size(); }

void Monitor::onStart(const IPosition::CPtr& startPosition)
{
  startTime = std::chrono::high_resolution_clock::now();
  globalMetrics.bestEnergy = startPosition->getEnergy();
}

void Monitor::onCandidate(const IPosition::CPtr& position, double delta, double energy, double progress)
{
  if (level > MonitorLevel::Low) {
    deltasCandidate.push(delta);
    energiesCandidate.push(energy);
    if (level == MonitorLevel::Medium) {
      addSnapshotChecked(position, progress);
    }
    if ((level == MonitorLevel::High) && (stalledAcceptance == 0) && (snapshotsMemory < snapshotsMemoryLimit)) {
      addSnapshot(position, progress);
    }
    ++stalledAcceptance;
  }
  ++globalMetrics.idx;
}

void Monitor::onAcceptance(const IPosition::CPtr& position, double delta, double energy, double progress)
{
  ++globalMetrics.acceptance;
  if (delta > 0) {
    ++globalMetrics.upEnergyChanges;
  }
  deltasAcceptance.push(delta);
  energiesAcceptance.push(energy);
  if ((energy < globalMetrics.bestEnergy) ||
      (bestCatchQ < progress && !bestPosition && (energy - globalMetrics.bestEnergy < catchPrecision))) {
    globalMetrics.bestEnergy = energy;
    globalMetrics.bestIdx = globalMetrics.idx;
    globalMetrics.bestProgress = progress;
    bestCatch(position, progress);
  }
  if (level > MonitorLevel::Low) {
    stalledAcceptance = 0;
  }
}

void Monitor::onEnd(const IPosition::CPtr& position)
{
  refreshGlobalMetrics(1.0);
  if (level > MonitorLevel::Low) {
    energiesCandidate.push(position->getEnergy());
    if (level == MonitorLevel::Medium || ((level == MonitorLevel::High) && (stalledAcceptance == 0))) {
      addSnapshot(position, 1.0);
    }
  }
}

void Monitor::refreshGlobalMetrics(double progress)
{
  auto elapsed = std::chrono::high_resolution_clock::now() - startTime;
  globalMetrics.duration = std::chrono::duration<double>(elapsed).count();
  globalMetrics.speed = double(globalMetrics.idx) / globalMetrics.duration;
  globalMetrics.progress = progress;
}

void Monitor::bestCatch(const IPosition::CPtr& position, double progress)
{
  if (bestCatchQ <= progress) {
    ++globalMetrics.bestCatch;
    bestPosition = position->clone();
  }
}

void Monitor::addSnapshot(const IPosition::CPtr& position, double progress)
{
  refreshGlobalMetrics(progress);
  snapshots.push_back(
      {position, globalMetrics, deltasCandidate, deltasAcceptance, energiesCandidate, energiesAcceptance});
  snapshotsMemory += snapshots.back().size();
  progressCallback(progress);
}

void Monitor::addSnapshotChecked(const IPosition::CPtr& position, double progress)
{
  if (((snapshots.size() < steps) && (snapshots.size() <= progress * double(steps)) &&
       (snapshotsMemory < snapshotsMemoryLimit)) ||
      (snapshots.size() == 0)) {
    addSnapshot(position, progress);
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
  os << "GlobalMetrics[" << "bestEnergy=" << gm.bestEnergy << ",bestIdx=" << gm.bestIdx << ",allIt=" << gm.idx
     << ",acceptedIt=" << gm.acceptance << ",upEnergyChanges=" << gm.upEnergyChanges << ",bestClone=" << gm.bestCatch
     << ",duration=" << gm.duration << "s,speed=" << int(gm.speed) << "/s" << "]";
  return os;
}

Monitor::CPtr Monitor::clone() const
{
  return std::make_unique<Monitor>(level, bestCatchQ, catchPrecision, localEnv, steps, snapshotsMemoryLimit,
                                   progressCallback);
}
