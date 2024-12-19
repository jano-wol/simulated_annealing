#ifndef SIMULATED_ANNEALING_GRAPHICS_MENUBAR_H_
#define SIMULATED_ANNEALING_GRAPHICS_MENUBAR_H_

#include <filesystem>
#include <string>
#include <vector>

#include <imgui/imgui.h>

struct File
{
  std::string alias;
  std::filesystem::path path;
};

class FileBrowser
{
public:
  FileBrowser(std::string title_, std::filesystem::path m_currentDirPath_);

  std::string title;
  std::filesystem::path currentDirPath;
  std::filesystem::path nextPath;
  int mode;
  bool visible;
  bool loadedVisible;
  int selection;
  std::vector<File> filesInScope;
  std::filesystem::path currPath;
  std::filesystem::path loadedPath;
  std::filesystem::path displayPath;
  char saveFileName[128];

  const ImVec4 disabledColor = {0.3f, 0.3f, 0.3f, 1.0f};
  const std::vector<std::string> buttonNames = {"", "Select", "Save"};

  void render();
  void renderActiveButton(const std::filesystem::path& outPath);
  void renderDisabledButton();
  void renderOpen();
  void renderSave();
};

#endif  // SIMULATED_ANNEALING_GRAPHICS_MENUBAR_H_
