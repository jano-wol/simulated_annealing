#include <io/Io.h>

#include <filesystem>
#include <fstream>
#include <sstream>

#include <core/IPosition.h>
#include <serializator/Serializator.h>

using namespace sa::core;
using namespace sa::io;
using namespace sa::serializator;

std::string getFileName(int idx)
{
  std::stringstream ss;
  ss << idx << ".txt";
  return ss.str();
}

std::string getFolderName(const IGenerator::CPtr& generator, const IPosition::CPtr& position)
{
  return std::filesystem::path(Io::getTargetPath()) / Serializator::getTypeId(position) / generator->getGeneratorName();
}

std::string getCorrespondingBest(const std::string& filePath)
{
  std::filesystem::path path(filePath);
  std::filesystem::path parentPath = path.parent_path();
  std::filesystem::path newPath = parentPath / "best" / path.filename();
  return newPath.string();
}

std::string Io::getWorkspaceRootPath()
{
  static std::string workspaceRootPath =
      std::filesystem::absolute(__FILE__).parent_path().parent_path().parent_path().parent_path().parent_path();
  return workspaceRootPath;
}

std::string Io::getDataPath()
{
  static std::string dataPath = std::filesystem::path(getWorkspaceRootPath()) / "data";
  return dataPath;
}

std::string Io::getTargetPath()
{
  static std::string targetPath = std::filesystem::path(getDataPath()) / "target";
  return targetPath;
}

void Io::savePosition(const std::string& pathStr, core::IPosition::CPtr& position)
{
  std::filesystem::path path(pathStr);
  std::filesystem::create_directories(path.parent_path());
  std::ofstream file(path);
  file << Serializator::toString(position);
}

void Io::savePosition(const core::IGenerator::CPtr& generator, int idx)
{
  auto position = generator->generatePosition(idx);
  std::filesystem::path folder(getFolderName(generator, position));
  std::filesystem::path filePath = folder.append(getFileName(idx));
  if (std::filesystem::exists(filePath)) {
    return;
  }
  savePosition(filePath, position); 
  std::filesystem::path bestPath(getCorrespondingBest(filePath));
  savePosition(bestPath, position);
}