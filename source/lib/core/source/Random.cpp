#include <core/Random.h>

#include <chrono>
#include <thread>

using namespace sa::core;

int Random::randomInt(int min, int max)
{
  std::uniform_int_distribution<int> dist(min, max);
  return dist(getThreadLocalGenerator());
}

double Random::randomDouble(double min, double max)
{
  std::uniform_real_distribution<double> dist(min, max);
  return dist(getThreadLocalGenerator());
}

void Random::timeInit()
{
  auto now = std::chrono::steady_clock::now();
  auto time_since_epoch = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
  getThreadLocalGenerator() = std::mt19937(static_cast<unsigned int>(time_since_epoch));
}

std::mt19937 Random::createThreadLocalGenerator()
{
  auto threadIdHash = std::hash<std::thread::id>()(std::this_thread::get_id());
  return std::mt19937(static_cast<unsigned int>(threadIdHash));
}

std::mt19937& Random::getThreadLocalGenerator()
{
  thread_local std::mt19937 generator = createThreadLocalGenerator();
  return generator;
}
