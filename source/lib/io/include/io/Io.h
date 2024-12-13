#ifndef SIMULATED_ANNEALING_IO_IO_H_
#define SIMULATED_ANNEALING_IO_IO_H_

#include <core/IPosition.h>

namespace sa::io
{
class Io
{
public:
  static void savePosition() {}
  static core::IPosition::CPtr loadPosition() { return nullptr; }
  static core::IPosition::CPtr proposeBest() { return nullptr; }

  static std::string workspaceRootPath;
  static std::string dataFolderPath;
};
}  // namespace sa::io

#endif  // SIMULATED_ANNEALING_CORE_IO_H_
