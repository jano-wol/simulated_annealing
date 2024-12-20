#ifndef SIMULATED_ANNEALING_GRAPHICS_MENU_UI_H_
#define SIMULATED_ANNEALING_GRAPHICS_MENU_UI_H_

#include <filesystem>
#include <future>
#include <vector>

#include <imgui/imgui.h>

#include <core/IPosition.h>

class MenuUI
{
public:
  MenuUI();

  struct File
  {
    std::string alias;
    std::filesystem::path path;
  };

  void startParsing();
  void startSaving(const sa::core::IPosition::CPtr& currPosition);
  void render();
  void renderActiveButton(const std::filesystem::path& outPath);
  void renderDisabledButton();
  void renderOpen();
  void renderSave();
  void menuUpdate();

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
  std::future<std::unique_ptr<sa::core::IPosition>> parsingFuture;
  std::future<void> savingFuture;
  const ImVec4 disabledColor = {0.3f, 0.3f, 0.3f, 1.0f};
  const std::vector<std::string> buttonNames = {"", "Select", "Save"};
};

#endif  // SIMULATED_ANNEALING_GRAPHICS_MENU_UI_H_
