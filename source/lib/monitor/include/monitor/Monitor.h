#ifndef SIMULATED_ANNEALING_MONITOR_MONITOR_H_
#define SIMULATED_ANNEALING_MONITOR_MONITOR_H_

#include <limits>
#include <sstream>
#include <string>
#include <vector>

#include <core/IMove.h>
#include <core/IPosition.h>
#include <core/MoveCandidate.h>

namespace sa::monitor
{

enum class MonitorLevel { Low, Medium, High };

class MonitorConfig
{
public:
  MonitorLevel level;
  size_t checkpointFrequency = 10000;
  size_t deltaHistorySize = 1000;
};

class SnapShot
{
  SnapShot() {}

  double localDerivative;
  std::pair<double, double> energyWindow;
  core::IPosition::CPtr position;
  Statistics deltaStats;
};

class Monitor
{
public:
  Monitor(MonitorConfig config_) : config(std::move(config_)) {}

  void candidatePhase() {}
  void acceptancePhase() {}
  std::string toString() const {}

private:
  MonitorConfig config;

  // low
  core::IPosition::CPtr bestPosition = nullptr;
  double bestEnergy = std::numeric_limits<double>::max();
  std::size_t bestIdx = 0;
  std::size_t idx = 0;
  std::size_t upEnergyChanges = 0;
  std::size_t acceptance = 0;
  std::size_t stalledAcceptance = 0;

  // medium
  double speed = 0;
  std::vector<double> deltas;
  std::vector<double> energies;
  std::vector<core::IPosition::CPtr> checkpoints;

  // high
  std::vector<core::IMove::CPtr> moves;
};
}  // namespace sa::monitor

#endif  // SIMULATED_ANNEALING_MONITOR_MONITOR_H_
