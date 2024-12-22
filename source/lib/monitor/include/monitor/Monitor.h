#ifndef SIMULATED_ANNEALING_MONITOR_MONITOR_H_
#define SIMULATED_ANNEALING_MONITOR_MONITOR_H_

#include <chrono>
#include <functional>
#include <sstream>
#include <string>
#include <vector>

#include <core/CircularBuffer.h>
#include <core/IPosition.h>
#include <core/Statistics.h>

namespace sa::monitor
{

enum class MonitorLevel { Low, Medium, High };

class GlobalMetrics
{
public:
  int size() const;

  double bestEnergy = 0;
  std::size_t bestIdx = 0;
  std::size_t idx = 0;
  std::size_t upEnergyChanges = 0;
  std::size_t acceptance = 0;
  std::size_t bestCatch = 0;
  double duration = 0;
  double speed = 0;
};

class Snapshot
{
public:
  Snapshot(const core::IPosition::CPtr& position_, GlobalMetrics globalMetrics_, const core::CircularBuffer& deltas,
           const core::CircularBuffer& energies);

  int size() const;

  double localDerivative;
  double minEnergy;
  double maxEnergy;
  core::Statistics deltaStats;
  GlobalMetrics globalMetrics;
  core::IPosition::CPtr position;
};

class Monitor
{
public:
  using CPtr = std::unique_ptr<Monitor>;
  using ProgressCallback = std::function<void(double)>;
  Monitor(
      MonitorLevel level_, double bestCatchQ_ = 0.9, double catchPrecision_ = 1e-6, std::size_t localEnv_ = 1000,
      std::size_t steps_ = 20, std::size_t snapshotsMemoryLimit_ = 2 * 1000000000UL,
      ProgressCallback progressCallback_ = [](double /*progress*/) {})
      : level(level_),
        bestCatchQ(bestCatchQ_),
        catchPrecision(catchPrecision_),
        localEnv(localEnv_),
        steps(steps_),
        snapshotsMemoryLimit(snapshotsMemoryLimit_),
        progressCallback(std::move(progressCallback_)),
        deltas(localEnv),
        energies(localEnv)
  {}

  void onStart(const core::IPosition::CPtr& startPosition);
  void onCandidate(const core::IPosition::CPtr& position, double delta, double energy, double progress);
  void onAcceptance(const core::IPosition::CPtr& position, double delta, double progress, double energy);
  void onEnd(const core::IPosition::CPtr& position);
  void bestCatch(const core::IPosition::CPtr& position, double progress);
  void addSnapshot(const core::IPosition::CPtr& position, double progress);
  void addSnapshotChecked(const core::IPosition::CPtr& position, double progress);
  void refreshGlobalMetrics();
  std::string toString() const;
  CPtr clone() const;

  MonitorLevel level;
  double bestCatchQ;
  double catchPrecision;
  std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
  std::size_t localEnv;
  std::size_t steps;
  std::size_t snapshotsMemoryLimit;
  ProgressCallback progressCallback;
  std::size_t snapshotsMemory = 0;

  // low
  core::IPosition::CPtr bestPosition = nullptr;
  GlobalMetrics globalMetrics;

  // medium
  std::size_t stalledAcceptance = 0;
  core::CircularBuffer deltas;
  core::CircularBuffer energies;
  std::vector<Snapshot> snapshots;
};

std::ostream& operator<<(std::ostream& os, const GlobalMetrics& globalMetrics);
}  // namespace sa::monitor

#endif  // SIMULATED_ANNEALING_MONITOR_MONITOR_H_
