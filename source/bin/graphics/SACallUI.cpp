#include "SACallUI.h"

#include <imgui/imgui.h>

void SACallUI::saCallUpdate()
{
  if (ImGui::Button("Simulate")) {
    saCalled = true;
  }
}

void SACallUI::startSimulating(const sa::core::IPosition::CPtr& currPosition, sa::sa::SA::CPtr& sa)
{
  simulatingFuture = std::async(std::launch::async, [&currPosition, &sa]() { sa->anneal(currPosition); });
}
