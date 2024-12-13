#ifndef SIMULATED_ANNEALING_IO_IO_H_
#define SIMULATED_ANNEALING_IO_IO_H_

#include <core/IGenerator.h>
#include <core/IPosition.h>

namespace sa::io
{
class Io
{
public:
  static void savePosition(const std::string& path, core::IPosition::CPtr& position);
  static void savePosition(const core::IGenerator::CPtr& generator, int idx);
  static core::IPosition::CPtr loadPosition(const core::IGenerator::CPtr& generator, int idx) { return nullptr; }
  static core::IPosition::CPtr proposeBest() { return nullptr; }

  static std::string getWorkspaceRootPath();
  static std::string getDataPath();
  static std::string getTargetsPath();
};
}  // namespace sa::io

#endif  // SIMULATED_ANNEALING_CORE_IO_H_
