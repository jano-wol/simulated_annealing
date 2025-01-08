#include "SACallUI.h"
#include "ThreadPoolManager.h"

#include <imgui/imgui.h>

#include <io/Io.h>

using namespace sa::core;
using namespace sa::io;
using namespace sa::sa;

void updateProgressBar(const std::deque<std::atomic<double>>& progresses, float width, float height)
{
  ImGui::SameLine();
  float totalFraction = 0;
  for (std::size_t barIdx = 0; barIdx < progresses.size(); ++barIdx) {
    float fraction = progresses[barIdx].load();
    totalFraction += fraction;
    ImGui::ProgressBar(fraction, ImVec2(width / progresses.size(), height), "");
    if (barIdx != progresses.size() - 1) {
      float gap = 2.0f;
      ImGui::SameLine(0.0f, gap);
    }
  }
}

SA::CPtr simulate(const IPosition::CPtr& currPosition, const std::vector<SAFactory::CPtr>& factories,
                  const std::uint64_t n)
{
  auto sa = factories[n]->create();
  IPosition::CPtr bestPosition = nullptr;
  sa->anneal(currPosition);
  return SA::CPtr{std::move(sa)};
}

void SACallUI::saCallUpdate(bool isAnnealing)
{
  ImVec2 cursorPos = ImGui::GetCursorScreenPos();
  ImVec2 availSize = ImGui::GetContentRegionAvail();
  float buttonWidth = availSize.x * 0.1f;
  float buttonHeight = availSize.y * 0.05f;
  ImVec2 buttonSize(buttonWidth, buttonHeight);
  ImU32 buttonColor = isAnnealing ? IM_COL32(120, 10, 10, 255) : IM_COL32(10, 120, 10, 255);
  std::string title = isAnnealing ? "Stop" : "Simulate";
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
    if (isAnnealing) {
      stop.store(true);
    } else {
      saCalled = true;
    }
  }
  if (isAnnealing) {
    updateProgressBar(progresses, buttonWidth * 2, buttonHeight);
  }
}

void SACallUI::startSimulating(const IPosition::CPtr& currPosition, std::vector<SAFactory::CPtr> factories_)
{
  std::size_t tasks = factories_.size();
  auto& pool = ThreadPoolManager::getPool();
  simulatingFutures =
      pool.submit_sequence(0, tasks, [&currPosition, factories = std::move(factories_)](std::uint64_t n) {
        return simulate(currPosition, factories, n);
      });
}

void SACallUI::postProcessResults(const IPosition::CPtr& allTimeBest, bool trackBest,
                                  const std::string& allTimeBestFile, std::vector<SA::CPtr> results_)
{
  resultFuture = std::async(
      std::launch::async, [&allTimeBest, trackBest, allTimeBestFile, results = std::move(results_)]() mutable {
        double bestEnergy = results[0]->getBest()->getEnergy();
        std::size_t bestIdx = 0;
        std::cout << bestEnergy << "\n";
        for (std::size_t i = 1; i < results.size(); ++i) {
          double currEnergy = results[i]->getBest()->getEnergy();
          std::cout << currEnergy << "\n";
          if (currEnergy < bestEnergy) {
            bestIdx = i;
            bestEnergy = currEnergy;
          }
        }
        IPosition::CPtr bestPosition = nullptr;
        if (trackBest) {
          const auto& currBestPosition = results[bestIdx]->getBest();
          if ((!allTimeBest) ||
              (currBestPosition->getEnergy() + results[bestIdx]->monitor->catchPrecision < allTimeBest->getEnergy())) {
            Io::savePosition(allTimeBestFile, currBestPosition);
            bestPosition = currBestPosition->clone();
          }
        }
        return std::pair<SA::CPtr, IPosition::CPtr>{std::move(results[bestIdx]), std::move(bestPosition)};
      });
}
