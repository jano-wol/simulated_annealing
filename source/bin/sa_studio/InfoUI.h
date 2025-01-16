#ifndef SIMULATED_ANNEALING_SA_STUDIO_INFO_UI_H_
#define SIMULATED_ANNEALING_SA_STUDIO_INFO_UI_H_

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
  std::string message;
};

#endif  // SIMULATED_ANNEALING_SA_STUDIO_INFO_UI_H_
