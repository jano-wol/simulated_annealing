#ifndef SIMULATED_ANNEALING_GRAPHICS_INFO_UI_H_
#define SIMULATED_ANNEALING_GRAPHICS_INFO_UI_H_

#include <filesystem>
#include <future>
#include <vector>

#include <imgui/imgui.h>

#include <core/IPosition.h>

class InfoUI
{
public:
  void infoUpdate();

  bool okPushed = false;
  std::string infoMessage;
};

#endif  // SIMULATED_ANNEALING_GRAPHICS_INFO_UI_H_
