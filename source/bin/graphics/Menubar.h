#ifndef SIMULATED_ANNEALING_GRAPHICS_MENUBAR_H_
#define SIMULATED_ANNEALING_GRAPHICS_MENUBAR_H_

#include <filesystem>
#include <string>
#include <vector>

namespace imgui_menubar
{
struct file
{
  std::string alias;
  std::filesystem::path path;
};

class file_browser_modal final
{
  const char* m_title;

  bool m_oldVisibility;

  int m_selection;

  std::filesystem::path m_currentPath;
  bool m_currentPathIsDir;

  std::vector<file> m_filesInScope;

public:
  file_browser_modal(const char* title);

  bool render(const bool isVisible, std::string& outPath);
};

void menuBarFile();

};  // namespace imgui_menubar

#endif  // SIMULATED_ANNEALING_GRAPHICS_MENUBAR_H_
