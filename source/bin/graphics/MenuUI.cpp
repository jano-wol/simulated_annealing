#include "MenuUI.h"

#include <algorithm>

#include <io/Io.h>

using namespace sa::core;
using namespace sa::io;

static void get_files_in_path(const std::filesystem::path& path, std::vector<MenuUI::File>& files)
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
  const std::vector<MenuUI::File>* v = reinterpret_cast<std::vector<MenuUI::File>*>(data);
  const int elementCount = clamp_size_t_to_int(v->size());
  if (idx < 0 || idx >= elementCount)
    return false;
  *out_text = v->at(idx).alias.data();
  return true;
}

MenuUI::MenuUI()
    : title("Open"),
      currentDirPath(Io::getTargetsPath()),
      nextPath(),
      mode(0),
      visible(false),
      loadedVisible(false),
      selection(-1)
{
  saveFileName[0] = '\0';
}

void MenuUI::renderActiveButton(const std::filesystem::path& outPath)
{
  bool activateButton = false;
  ImGui::Spacing();
  if (ImGui::Button(buttonNames[mode].c_str())) {
    activateButton = true;
  }
  if (ImGui::IsKeyPressed(ImGuiKey_Enter)) {
    activateButton = true;
  }
  if (activateButton) {
    ImGui::CloseCurrentPopup();
    loadedPath = outPath;
    operationRequest = true;
    visible = false;
    nextPath.clear();
    saveFileName[0] = '\0';
    selection = -1;
  }
}

void MenuUI::renderDisabledButton()
{
  ImGui::Spacing();
  ImGui::PushStyleColor(ImGuiCol_Button, disabledColor);
  ImGui::PushStyleColor(ImGuiCol_ButtonActive, disabledColor);
  ImGui::PushStyleColor(ImGuiCol_ButtonHovered, disabledColor);
  ImGui::Button(buttonNames[mode].c_str());
  ImGui::PopStyleColor(3);
}

void MenuUI::renderOpen()
{
  if (!nextPath.empty() && !std::filesystem::is_directory(nextPath)) {
    renderActiveButton(nextPath);
    ImGui::SameLine();
    ImGui::Checkbox("Enable all-time-best tracking", &trackBestCandidate);
  } else {
    renderDisabledButton();
  }
  displayPath = nextPath.empty() ? currentDirPath : nextPath;
}

void MenuUI::renderSave()
{
  ImGui::Spacing();
  ImGui::Text("File name:");
  ImGui::SameLine();
  ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * (1.0 / 3.0));
  ImGui::InputText("##InputSaveFileName", saveFileName, IM_ARRAYSIZE(saveFileName));
  std::filesystem::path savePath = currentDirPath / saveFileName;

  if (!std::filesystem::is_directory(savePath)) {
    renderActiveButton(savePath);
  } else {
    renderDisabledButton();
  }
  displayPath = savePath;
}

void MenuUI::render()
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
  if (ImGui::BeginPopupModal(title.c_str(), &isOpen, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse)) {
    if (ImGui::ListBox("##", &selection, vector_file_items_getter, &filesInScope, filesInScope.size(), 10)) {
      nextPath = filesInScope[selection].path;
      strncpy(saveFileName, nextPath.filename().string().c_str(), IM_ARRAYSIZE(saveFileName));
      strncpy(bestFileName, Io::getCorrespondingBest(nextPath.string()).c_str(), IM_ARRAYSIZE(bestFileName));
    }
    if (!nextPath.empty() && std::filesystem::is_directory(nextPath)) {
      get_files_in_path(nextPath, filesInScope);
      currentDirPath = nextPath;
      saveFileName[0] = '\0';
      selection = -1;
      nextPath.clear();
    }
    if (mode == 1) {
      renderOpen();
    }
    if (mode == 2) {
      renderSave();
    }
    float lineHeight = ImGui::GetTextLineHeight();
    ImVec2 childSize(ImGui::GetContentRegionAvail().x, 4 * lineHeight);
    ImGui::BeginChild("PathDisplayRegion", childSize, true, ImGuiWindowFlags_HorizontalScrollbar);
    ImGui::TextWrapped("Path: %s", displayPath.string().c_str());

    ImGui::SetScrollHereY(1.0f);
    ImGui::EndChild();

    ImGui::EndPopup();
  }
}

void MenuUI::menuUpdate()
{
  loadedVisible = false;
  operationRequest = false;
  if (ImGui::BeginMenuBar()) {
    if (ImGui::BeginMenu("File")) {
      if (ImGui::MenuItem("Open")) {
        loadedVisible = true;
        title = "Open";
        mode = 1;
      }
      if (ImGui::MenuItem("Save")) {
        loadedVisible = true;
        saveFileName[0] = '\0';
        title = "Save";
        mode = 2;
      }
      ImGui::EndMenu();
    }
    {
      ImGuiIO& io = ImGui::GetIO();
      ImGui::SetCursorPosX(ImGui::GetWindowWidth() * (9.0 / 10.0));
      ImGui::Text("FPS: %.1f", io.Framerate);
    }
    ImGui::EndMenuBar();
  }
  render();
}

void MenuUI::startParsing()
{
  parsingFuture = std::async(std::launch::async, [this]() {
    IPosition::CPtr best;
    if (trackBest) {
      best = Io::getPosition(bestFileName);
    }
    return std::pair<IPosition::CPtr, IPosition::CPtr>{std::move(best), Io::getPosition(loadedPath.string())};
  });
}

void MenuUI::startSaving(const IPosition::CPtr& currPosition)
{
  savingFuture = std::async(std::launch::async, [this, &currPosition]() {
    Io::savePosition(loadedPath.string(), currPosition);
    if (std::filesystem::exists(loadedPath) && std::filesystem::is_regular_file(loadedPath) &&
        (std::filesystem::file_size(loadedPath) != 0)) {
      return true;
    }
    return false;
  });
}
