#ifndef SIMULATED_ANNEALING_IO_DIRECTORY_ITERATOR_H_
#define SIMULATED_ANNEALING_IO_DIRECTORY_ITERATOR_H_

#include <filesystem>
#include <functional>
#include <string>

namespace sa::io
{
class DirectoryIterator
{
public:
  DirectoryIterator(std::string rootPath_) : rootPath(std::move(rootPath_)) {}

  void iterate(std::function<void(const std::string&)> callback) const;

private:
  std::string rootPath;
};
}  // namespace sa::io

#endif  // SIMULATED_ANNEALING_IO_DIRECTORY_ITERATOR_H_
