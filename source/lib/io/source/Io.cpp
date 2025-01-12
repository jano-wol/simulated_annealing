#include <io/Io.h>

#include <filesystem>
#include <fstream>
#include <sstream>

#include <core/IPosition.h>
#include <core/Rounding.h>
#include <io/DirectoryIterator.h>
#include <serializator/Serializator.h>

using namespace sa::core;
using namespace sa::io;
using namespace sa::serializator;

namespace
{
std::string getFileName(int idx)
{
  std::stringstream ss;
  ss << idx << ".txt";
  return ss.str();
}

std::string getFolderName(const IGenerator::CPtr& generator, const IPosition::CPtr& position)
{
  return (std::filesystem::path(Io::getTargetsPath()) / position->getTypeId() / generator->getGeneratorName()).string();
}

std::pair<std::string, IPosition::CPtr> getFilePathAndPosition(const IGenerator::CPtr& generator, int idx)
{
  auto position = generator->generatePosition(idx);
  std::filesystem::path folder(getFolderName(generator, position));
  std::filesystem::path filePath = folder / getFileName(idx);
  return {filePath.string(), std::move(position)};
}
}  // namespace

std::string Io::getWorkspaceRootPath()
{
  static std::string workspaceRootPath =
      std::filesystem::absolute(__FILE__).parent_path().parent_path().parent_path().parent_path().parent_path().string();
  return workspaceRootPath;
}

std::string Io::getSourcePath()
{
  static std::string sourcePath = (std::filesystem::path(getWorkspaceRootPath()) / "source").string();
  return sourcePath;
}

std::string Io::getExtPath()
{
  static std::string extPath = (std::filesystem::path(getSourcePath()) / "ext").string();
  return extPath;
}

std::string Io::getFontsPath()
{
  static std::string fontsPath = (std::filesystem::path(getExtPath()) / "imgui" / "imgui" / "fonts").string();
  return fontsPath;
}

std::string Io::getFontPath()
{
  static std::string fontsPath = (std::filesystem::path(getFontsPath()) / "Cousine-Regular.ttf").string();
  return fontsPath;
}

std::string Io::getDataPath()
{
  static std::string dataPath = (std::filesystem::path(getWorkspaceRootPath()) / "data").string();
  return dataPath;
}

std::string Io::getTargetsPath()
{
  static std::string targetsPath = (std::filesystem::path(getDataPath()) / "targets").string();
  return targetsPath;
}

std::string Io::getTargetPath(const std::string& target)
{
  std::string targetPath = (std::filesystem::path(getTargetsPath()) / target).string();
  return targetPath;
}

void Io::savePosition(const std::string& pathStr, const IPosition::CPtr& position)
{
  std::filesystem::path path(pathStr);
  std::filesystem::create_directories(path.parent_path());
  std::ofstream file(path);
  file << Serializator::toString(position);
}

void Io::savePosition(const core::IGenerator::CPtr& generator, int idx)
{
  auto [filePath, position] = getFilePathAndPosition(generator, idx);
  if (std::filesystem::exists(filePath)) {
    return;
  }
  savePosition(filePath, position);
}

IPosition::CPtr Io::getPosition(const IGenerator::CPtr& generator, int idx)
{
  auto [filePath, position] = getFilePathAndPosition(generator, idx);
  if (std::filesystem::exists(filePath)) {
    return getPosition(filePath);
  }
  return std::move(position);
}

IPosition::CPtr Io::getPosition(const std::string& path)
{
  if (!std::filesystem::exists(path)) {
    return nullptr;
  }
  std::ifstream file(path);
  std::string line;
  std::getline(file, line);
  return Serializator::fromString(line);
}

std::string Io::getPath(const IGenerator::CPtr& generator, int idx)
{
  auto [filePath, position] = getFilePathAndPosition(generator, idx);
  return filePath;
}

void Io::tryImproveBest(const IGenerator::CPtr& generator, int idx, const IPosition::CPtr& bestCandidate)
{
  auto [filePath, position] = getFilePathAndPosition(generator, idx);
  tryImproveBest(filePath, bestCandidate);
}

std::string Io::getCorrespondingBest(const std::string& filePath)
{
  std::filesystem::path path(filePath);
  std::filesystem::path parentPath = path.parent_path();
  std::filesystem::path newPath = parentPath / "best" / path.filename();
  return newPath.string();
}

void Io::tryImproveBest(const std::string& positionPath, const core::IPosition::CPtr& bestCandidate)
{
  std::string bestPath = getCorrespondingBest(positionPath);
  auto currBest = Io::getPosition(bestPath);
  if (currBest == nullptr || bestCandidate->getEnergy() + precision < currBest->getEnergy()) {
    savePosition(bestPath, bestCandidate);
  }
}

void Io::refreshBest(const std::string& path)
{
  std::filesystem::path root(path);
  DirectoryIterator dirIter(path);
  std::ofstream file(root / "BestSummary.txt");
  dirIter.iterate([&file, &root](const std::string& filePath) {
    std::filesystem::path p(filePath);
    if (p.parent_path().filename() == "best" && p.extension() == ".txt") {
      auto position = getPosition(p.string());
      file << p.lexically_relative(root).string() << " " << Rounding::roundDouble(position->getEnergy()) << "\n";
    }
  });
}
