#include <core/Random.h>

#include <chrono>

using namespace sa::core;

int Random::seed = 0;
thread_local int Random::effectiveSeed = 0;

void Random::setSeed(int s) { seed = s; }

void Random::setEffectiveSeed(int taskIdx) { effectiveSeed = seed ^ (taskIdx * 31); }

int Random::randomInt(int min, int max) const
{
  std::uniform_int_distribution<int> dist(min, max);
  return dist(r);
}

double Random::randomDouble(double min, double max) const
{
  std::uniform_real_distribution<double> dist(min, max);
  return dist(r);
}
