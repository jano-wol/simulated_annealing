#include <policies/Resource.h>

#include <sstream>

using namespace sa::policies;

void Iteration::onStart() {}

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
  ss << "Iteration[" << std::size_t(all) << "]";
  return ss.str();
}

IResource::CPtr Iteration::clone() const { return std::make_unique<Iteration>(all); }

void Time::onStart() { startTime = std::chrono::high_resolution_clock::now(); }

double Time::getAll() const { return all.count(); }

double Time::getLeft() const
{
  if (state == 0) {
    state = quant;
    auto elapsed = std::chrono::high_resolution_clock::now() - startTime;
    double elapsedSeconds = std::chrono::duration<double>(elapsed).count();
    lastRet = std::max(0.0, all.count() - elapsedSeconds);
    return lastRet;
  } else {
    --state;
    return lastRet;
  }
}

void Time::updateLeft() {}

std::string Time::toString() const
{
  std::stringstream ss;
  ss << "Time[" << std::size_t(all.count()) << "s,quant=" << quant << "]";
  return ss.str();
}

IResource::CPtr Time::clone() const { return std::make_unique<Time>(all.count(), quant); }
