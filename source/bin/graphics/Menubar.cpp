#include "Menubar.h"

#include <algorithm>

#include <imgui/imgui.h>
#include <io/Io.h>

using namespace imgui_menubar;
using namespace sa::io;

static void get_files_in_path(const std::filesystem::path& path, std::vector<file>& files)
{
  files.clear();

  if (path.has_parent_path()) {
    files.push_back({"..", path.parent_path()});
  }

  std::vector<std::pair<std::string, std::filesystem::path>> f;
  for (const std::filesystem::directory_entry& dirEntry : std::filesystem::directory_iterator(path)) {
    const std::filesystem::path& dirPath = dirEntry.path();
    f.push_back({dirPath.filename().string(), dirEntry.path()});
  }
  std::sort(f.begin(), f.end(), [](const auto& a, const auto& b) { return a.first < b.first; });
  for (const auto& [a, b] : f) {
    files.push_back({a, b});
  }
}

static int clamp_size_t_to_int(const size_t data)
{
  static const int max_int = std::numeric_limits<int>::max();
  return static_cast<int>(data > max_int ? max_int : data);
}

static bool vector_file_items_getter(void* data, int idx, const char** out_text)
{
  const std::vector<file>* v = reinterpret_cast<std::vector<file>*>(data);
  const int elementCount = clamp_size_t_to_int(v->size());
  if (idx < 0 || idx >= elementCount)
    return false;
  *out_text = v->at(idx).alias.data();
  return true;
}

file_browser_modal::file_browser_modal(const char* title, std::filesystem::path m_currentPath_)
    : m_title(title), m_oldVisibility(false), m_selection(0), m_currentPath(m_currentPath_), m_currentPathIsDir(true)
{}

// Will return true if file selected.
bool file_browser_modal::render(const bool isVisible, std::string& outPath)
{
  bool result = false;
  if (m_oldVisibility != isVisible) {
    m_oldVisibility = isVisible;
    // Visiblity has changed.

    if (isVisible) {
      // Only run when the visibility state changes to visible.

      // Reset the path to the initial path.
      m_currentPath = Io::getTargetsPath();
      m_currentPathIsDir = true;

      // Update paths based on current path
      get_files_in_path(m_currentPath, m_filesInScope);

      // Make the modal visible.
      ImGui::OpenPopup(m_title);
    }
  }

  bool isOpen = true;
  float minWidth = ImGui::GetWindowWidth() * (2.0 / 3.0);
  float maxWidth = ImGui::GetWindowWidth();
  float maxHeight = ImGui::GetWindowHeight() * 0.8f;
  ImGui::SetNextWindowSizeConstraints(ImVec2(minWidth, -1), ImVec2(maxWidth, maxHeight));
  ImGui::SetNextWindowPos({0, 0});
  if (ImGui::BeginPopupModal(m_title, &isOpen,
                             ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar)) {
    // ListBox for selecting files/directories
    if (ImGui::ListBox("##", &m_selection, vector_file_items_getter, &m_filesInScope, m_filesInScope.size(), 10)) {
      m_currentPath = m_filesInScope[m_selection].path;
      m_currentPathIsDir = std::filesystem::is_directory(m_currentPath);

      if (m_currentPathIsDir) {
        get_files_in_path(m_currentPath, m_filesInScope);
      }
    }
    ImGui::Spacing();
    // "Select" button logic
    if (m_currentPathIsDir) {
      static const ImVec4 disabledColor = {0.3f, 0.3f, 0.3f, 1.0f};
      ImGui::PushStyleColor(ImGuiCol_Button, disabledColor);
      ImGui::PushStyleColor(ImGuiCol_ButtonActive, disabledColor);
      ImGui::PushStyleColor(ImGuiCol_ButtonHovered, disabledColor);
      ImGui::Button("Select");
      ImGui::PopStyleColor(3);
    } else {
      if (ImGui::Button("Select")) {
        ImGui::CloseCurrentPopup();
        outPath = m_currentPath.string();
        result = true;
      }
    }
    ImGui::Spacing();
    ImVec2 childSize(ImGui::GetContentRegionAvail().x, 100.0f);
    ImGui::BeginChild("PathDisplayRegion", childSize, true, ImGuiWindowFlags_HorizontalScrollbar);
    ImGui::TextWrapped("Current Path: %s", m_currentPath.string().c_str());
    ImGui::SetScrollHereY(1.0f);
    ImGui::EndChild();

    ImGui::EndPopup();
  }
  return result;
}

void normalize(std::string& path)
{
  for (size_t i = 0; i < path.size(); i++) {
    if (path[i] == '\\') {
      path[i] = '/';
    }
  }
}

std::string imgui_menubar::menuBarFile()
{
  int isImportClicked = false;
  if (ImGui::BeginMenuBar()) {
    if (ImGui::BeginMenu("File")) {
      if (ImGui::MenuItem("Open")) {
        isImportClicked = true;
      }
      ImGui::EndMenu();
    }
    ImGui::EndMenuBar();
  }

  static file_browser_modal fileBrowser("Open", Io::getTargetsPath());

  std::string path = "";
  if (fileBrowser.render(isImportClicked, path)) {
    normalize(path);
  }
  return path;
}