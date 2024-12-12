#ifndef SIMULATED_ANNEALING_CORE_IO_H_
#define SIMULATED_ANNEALING_CORE_IO_H_

#include <core/IPosition.h>

namespace sa::core
{
class Io
{
public:
  static void savePosition() {}
  static IPosition::CPtr loadPosition() { return nullptr; }
  static IPosition::CPtr proposeBest() { return nullptr; }

  static std::string workspaceRootPath;
  static std::string dataFolderPath;
};
}  // namespace sa::core

#endif  // SIMULATED_ANNEALING_CORE_IO_H_
