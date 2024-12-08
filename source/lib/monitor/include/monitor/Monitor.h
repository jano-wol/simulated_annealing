#ifndef SIMULATED_ANNEALING_MONITOR_MONITOR_H_
#define SIMULATED_ANNEALING_MONITOR_MONITOR_H_

#include <chrono>
#include <sstream>
#include <string>
#include <vector>

#include <core/CircularBuffer.h>
#include <core/IMove.h>
#include <core/IPosition.h>
#include <core/Statistics.h>

namespace sa::monitor
{

enum class MonitorLevel { Low, Medium, High };

class SnapShot
{
  SnapShot() {}

  double localDerivative;
  std::pair<double, double> energyWindow;
  core::IPosition::CPtr position;
  core::Statistics deltaStats;
};

class GlobalMetrics
{
public:
  double bestEnergy = 0;
  std::size_t bestIdx = 0;
  std::size_t idx = 0;
  std::size_t upEnergyChanges = 0;
  std::size_t acceptance = 0;
  std::size_t bestCatch = 0;
  double duration = 0;
  double speed = 0;
};

class Monitor
{
public:
  Monitor(MonitorLevel level_, std::size_t localEnv_ = 1000, double bestCatchQ_ = 0.9, double catchPrecision_ = 1e-6)
      : level(level_),
        localEnv(localEnv_),
        bestCatchQ(bestCatchQ_),
        catchPrecision(catchPrecision_),
        deltas(localEnv),
        energies(localEnv)
  {}

  void onStart(const core::IPosition::CPtr& startPosition);
  void onCandidate(const core::IMove::CPtr& move, double delta);
  void onAcceptance(const core::IPosition::CPtr& position, double delta, double progress);
  void onEnd();
  void bestCatch(const core::IPosition::CPtr& position, double progress);
  std::string toString() const;

  MonitorLevel level;
  std::size_t localEnv;
  double bestCatchQ;
  double catchPrecision;
  std::chrono::time_point<std::chrono::high_resolution_clock> startTime;

  // low
  core::IPosition::CPtr bestPosition = nullptr;
  GlobalMetrics globalMetrics;

  // medium
  std::size_t stalledAcceptance = 0;
  core::CircularBuffer deltas;
  core::CircularBuffer energies;
  std::vector<core::IPosition::CPtr> checkpoints;

  // high
  std::vector<core::IMove::CPtr> moves;
};

std::ostream& operator<<(std::ostream& os, const GlobalMetrics& globalMetrics);
}  // namespace sa::monitor

#endif  // SIMULATED_ANNEALING_MONITOR_MONITOR_H_
