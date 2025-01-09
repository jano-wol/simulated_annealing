#include "InfoUI.h"

#include <imgui/imgui.h>

void InfoUI::infoUpdate()
{
  okPushed = false;
  ImGui::OpenPopup("Info");
  ImGuiStyle& style = ImGui::GetStyle();
  float size = (ImGui::CalcTextSize(message.c_str()).x + style.WindowPadding.x * 2.0f);
  ImGui::SetNextWindowSizeConstraints(ImVec2(size, 0), ImVec2(size, FLT_MAX));
  if (ImGui::BeginPopupModal("Info", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
    ImGui::TextWrapped("%s", message.c_str());
    if (ImGui::Button("OK", ImVec2(120, 0))) {
      okPushed = true;
      ImGui::CloseCurrentPopup();
    }

    ImGui::EndPopup();
  }
}