#include <core/CircularBuffer.h>

using namespace sa::core;

void CircularBuffer::push(double value)
{
  buffer[head] = value;
  head = (head + 1) % buffer.size();
  if (count < buffer.size()) {
    ++count;
  }
}

double CircularBuffer::getNewest() const
{
  std::size_t newestIndex = (head == 0) ? buffer.size() - 1 : head - 1;
  return buffer[newestIndex];
}

double CircularBuffer::getOldest() const
{
  std::size_t oldestIndex = (head + buffer.size() - count) % buffer.size();
  return buffer[oldestIndex];
}

bool CircularBuffer::isEmpty() const { return count == 0; }

std::span<const double> CircularBuffer::getData() const { return std::span<const double>(buffer.data(), count); }

std::size_t CircularBuffer::getCount() const { return count; }