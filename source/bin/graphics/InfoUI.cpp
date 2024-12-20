#include "InfoUI.h"

#include <imgui/imgui.h>

void InfoUI::infoUpdate()
{
  okPushed = false;
  ImGui::OpenPopup("Info");

  if (ImGui::BeginPopupModal("Info", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
    ImGui::TextWrapped("%s", message.c_str());
    ImGui::Separator();

    if (ImGui::Button("OK", ImVec2(120, 0))) {
      okPushed = true;
      ImGui::CloseCurrentPopup();
    }

    ImGui::EndPopup();
  }
}