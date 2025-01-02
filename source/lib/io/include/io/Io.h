#ifndef SIMULATED_ANNEALING_IO_IO_H_
#define SIMULATED_ANNEALING_IO_IO_H_

#include <core/IGenerator.h>
#include <core/IPosition.h>

namespace sa::io
{
class Io
{
public:
  static constexpr double precision = 1e-4;
  static std::string getWorkspaceRootPath();
  static std::string getSourcePath();
  static std::string getExtPath();
  static std::string getFontsPath();
  static std::string getFontPath();
  static std::string getDataPath();
  static std::string getTargetsPath();
  static std::string getTargetPath(const std::string& target);
  static std::string getCorrespondingBest(const std::string& filePath);
  static void savePosition(const std::string& path, const core::IPosition::CPtr& position);
  static void savePosition(const core::IGenerator::CPtr& generator, int idx);
  static core::IPosition::CPtr getPosition(const core::IGenerator::CPtr& generator, int idx);
  static core::IPosition::CPtr getPosition(const std::string& path);
  static void tryImproveBest(const core::IGenerator::CPtr& generator, int idx,
                             const core::IPosition::CPtr& bestCandidate);
  static void tryImproveBest(const std::string& positionPath, const core::IPosition::CPtr& bestCandidate);
  static std::string getPath(const core::IGenerator::CPtr& generator, int idx);
  static void refreshBest(const std::string& path);
};
}  // namespace sa::io

#endif  // SIMULATED_ANNEALING_CORE_IO_H_
