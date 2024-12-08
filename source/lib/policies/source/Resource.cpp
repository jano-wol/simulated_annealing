#include <policies/Resource.h>

#include <sstream>

using namespace sa::policies;

double Iteration::getAll() const { return all; }

double Iteration::getLeft() const { return left; }

void Iteration::updateLeft()
{
  if (left > 0) {
    --left;
  }
}
std::string Iteration::toString() const
{
  std::stringstream ss;
  ss << "Iteration=" << std::size_t(all);
  return ss.str();
}

double Time::getAll() const { return all.count(); }

double Time::getLeft() const
{
  auto elapsed = std::chrono::high_resolution_clock::now() - startTime;
  double elapsedSeconds = std::chrono::duration<double>(elapsed).count();
  double ret = std::max(0.0, all.count() - elapsedSeconds);
  return ret;
}

void Time::updateLeft() {}

std::string Time::toString() const
{
  std::stringstream ss;
  ss << "Time=" << std::size_t(all.count()) << "s";
  return ss.str();
}
