#include "SACallUI.h"

#include <imgui/imgui.h>

void SACallUI::saCallUpdate()
{
  ImVec2 cursorPos = ImGui::GetCursorScreenPos();
  ImVec2 availSize = ImGui::GetContentRegionAvail();
  float buttonWidth = availSize.x * 0.1f;
  float buttonHeight = availSize.y * 0.05f;
  ImVec2 buttonSize(buttonWidth, buttonHeight);
  ImU32 buttonColor = IM_COL32(10, 120, 10, 255);
  float rounding = 12.0f;
  ImDrawList* drawList = ImGui::GetWindowDrawList();
  ImVec2 buttonMin = cursorPos;
  ImVec2 buttonMax = ImVec2(buttonMin.x + buttonWidth, buttonMin.y + buttonHeight);
  ImGui::InvisibleButton("RoundedButton", buttonSize);
  drawList->AddRectFilled(buttonMin, buttonMax, buttonColor, rounding);
  ImVec2 textSize = ImGui::CalcTextSize("Simulate");
  ImVec2 textPos =
      ImVec2(buttonMin.x + (buttonWidth - textSize.x) * 0.5f, buttonMin.y + (buttonHeight - textSize.y) * 0.5f);
  drawList->AddText(textPos, IM_COL32(255, 255, 255, 255), "Simulate");
  if (ImGui::IsItemClicked()) {
    saCalled = true;
  }
}

void SACallUI::startSimulating(const sa::core::IPosition::CPtr& currPosition, sa::sa::SA::CPtr& sa)
{
  simulatingFuture = std::async(std::launch::async, [&currPosition, &sa]() { sa->anneal(currPosition); });
}
