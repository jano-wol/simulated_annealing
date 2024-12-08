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

std::span<const double> CircularBuffer::getData() const { return std::span<const double>(buffer.data(), count); }
