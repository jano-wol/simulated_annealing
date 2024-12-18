#include "Menubar.h"

#include <algorithm>

#include <imgui/imgui.h>
#include <io/Io.h>

using namespace sa::io;

static void get_files_in_path(const std::filesystem::path& path, std::vector<File>& files)
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
  const std::vector<File>* v = reinterpret_cast<std::vector<File>*>(data);
  const int elementCount = clamp_size_t_to_int(v->size());
  if (idx < 0 || idx >= elementCount)
    return false;
  *out_text = v->at(idx).alias.data();
  return true;
}

FileBrowser::FileBrowser(std::string title_, std::filesystem::path currentDirPath_)
    : title(std::move(title_)),
      currentDirPath(std::move(currentDirPath_)),
      visible(false),
      loadedVisible(false),
      selection(0)
{}

void FileBrowser::render()
{
  if (visible != loadedVisible) {
    visible = loadedVisible;
    if (visible) {
      get_files_in_path(currentDirPath, filesInScope);
      ImGui::OpenPopup(title.c_str());
    }
  }

  bool isOpen = true;
  float minWidth = ImGui::GetWindowWidth() * (2.0 / 3.0);
  float maxWidth = ImGui::GetWindowWidth();
  float maxHeight = ImGui::GetWindowHeight() * 0.8f;
  ImGui::SetNextWindowSizeConstraints(ImVec2(minWidth, -1), ImVec2(maxWidth, maxHeight));
  ImGui::SetNextWindowPos({0, 0});
  if (ImGui::BeginPopupModal(title.c_str(), &isOpen,
                             ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar)) {
    std::filesystem::path nextPath;
    bool nextPathIsDir;
    if (ImGui::ListBox("##", &selection, vector_file_items_getter, &filesInScope, filesInScope.size(), 10)) {
      nextPath = filesInScope[selection].path;
      nextPathIsDir = std::filesystem::is_directory(nextPath);
      if (nextPathIsDir) {
        get_files_in_path(nextPath, filesInScope);
      }
    }
    ImGui::Spacing();
    if (nextPathIsDir) {
      static const ImVec4 disabledColor = {0.3f, 0.3f, 0.3f, 1.0f};
      ImGui::PushStyleColor(ImGuiCol_Button, disabledColor);
      ImGui::PushStyleColor(ImGuiCol_ButtonActive, disabledColor);
      ImGui::PushStyleColor(ImGuiCol_ButtonHovered, disabledColor);
      ImGui::Button("Select");
      ImGui::PopStyleColor(3);
      currentDirPath = nextPath;
    } else {
      if (ImGui::Button("Select")) {
        ImGui::CloseCurrentPopup();
        loadedPath = nextPath;
        visible = false;
      }
    }
    ImGui::Spacing();
    ImVec2 childSize(ImGui::GetContentRegionAvail().x, 100.0f);
    ImGui::BeginChild("PathDisplayRegion", childSize, true, ImGuiWindowFlags_HorizontalScrollbar);
    ImGui::TextWrapped("Current Path: %s", currentDirPath.string().c_str());
    ImGui::SetScrollHereY(1.0f);
    ImGui::EndChild();

    ImGui::EndPopup();
  }
}

void normalize(std::string& path)
{
  for (size_t i = 0; i < path.size(); i++) {
    if (path[i] == '\\') {
      path[i] = '/';
    }
  }
}
