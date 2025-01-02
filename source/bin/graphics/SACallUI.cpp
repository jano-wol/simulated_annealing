#include "SACallUI.h"

#include <imgui/imgui.h>

#include <io/Io.h>

using namespace sa::io;

void updateProgressBar(double progress, float width, float height)
{
  ImGui::SameLine();
  ImGui::ProgressBar(progress, ImVec2(width, height), "");
  ImVec2 p = ImGui::GetItemRectMin();
  ImVec2 size = ImGui::GetItemRectSize();
  double percentage = progress * 100.0f;
  std::stringstream ss;
  ss.precision(1);
  ss << std::fixed << percentage << "%";
  std::string progressText = ss.str();
  ImDrawList* draw_list = ImGui::GetWindowDrawList();
  ImVec2 textSize = ImGui::CalcTextSize(progressText.c_str());
  ImVec2 textPos(p.x + (size.x - textSize.x) * 0.5f, p.y + (size.y - textSize.y) * 0.5f);
  draw_list->AddText(textPos, IM_COL32(255, 255, 255, 255), progressText.c_str());
}

void SACallUI::saCallUpdate(bool isSimulating)
{
  ImVec2 cursorPos = ImGui::GetCursorScreenPos();
  ImVec2 availSize = ImGui::GetContentRegionAvail();
  float buttonWidth = availSize.x * 0.1f;
  float buttonHeight = availSize.y * 0.05f;
  ImVec2 buttonSize(buttonWidth, buttonHeight);
  ImU32 buttonColor = isSimulating ? IM_COL32(120, 10, 10, 255) : IM_COL32(10, 120, 10, 255);
  std::string title = isSimulating ? "Stop" : "Simulate";
  float rounding = 12.0f;
  ImDrawList* drawList = ImGui::GetWindowDrawList();
  ImVec2 buttonMin = cursorPos;
  ImVec2 buttonMax = ImVec2(buttonMin.x + buttonWidth, buttonMin.y + buttonHeight);
  ImGui::InvisibleButton("RoundedButton", buttonSize);
  drawList->AddRectFilled(buttonMin, buttonMax, buttonColor, rounding);
  ImVec2 textSize = ImGui::CalcTextSize(title.c_str());
  ImVec2 textPos =
      ImVec2(buttonMin.x + (buttonWidth - textSize.x) * 0.5f, buttonMin.y + (buttonHeight - textSize.y) * 0.5f);
  drawList->AddText(textPos, IM_COL32(255, 255, 255, 255), title.c_str());
  if (ImGui::IsItemClicked()) {
    if (isSimulating) {
      stop.store(true);
    } else {
      saCalled = true;
    }
  }
  if (isSimulating) {
    updateProgressBar(progress.load(), buttonWidth * 2, buttonHeight);
  }
}

void SACallUI::startSimulating(const sa::core::IPosition::CPtr& currPosition, sa::core::IPosition::CPtr& allTimeBest,
                               bool trackBest, const std::string& allTimeBestFile, sa::sa::SA::CPtr& sa,
                               BS::thread_pool<0>& pool)
{
  simulatingFuture = pool.submit_task([&currPosition, &allTimeBest, trackBest, &allTimeBestFile, &sa]() {
    sa->anneal(currPosition);
    if (trackBest) {
      const auto& currBestPosition = sa->getBest();
      if ((!allTimeBest) || (currBestPosition->getEnergy() < allTimeBest->getEnergy())) {
        allTimeBest = currBestPosition->clone();
        Io::savePosition(allTimeBestFile, allTimeBest);
      }
    }
  });
}
