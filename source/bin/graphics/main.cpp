#include "GLFWInit.h"
#include "GLFWStop.h"
#include "LoopStart.h"
#include "MenuUI.h"
#include "Render.h"
#include "SAFactoryUI.h"
#include "Scale.h"
#include "StateUI.h"
#include "WindowBegin.h"
#include "WindowEnd.h"
#include "Targets.h"

int main()
{
  GLFWwindow* window = GLFWInit("SA");
  scale();
  StateUI state;
  while (!glfwWindowShouldClose(window)) {
    loopStart();
    windowBegin();
    state.handleInfo();
    state.handleMenu();
    state.handleSAFactory();
    state.handleSACall();
    state.handleSAOutput();
    windowEnd();
    render(window);
  }
  GLFWStop(window);
  return 0;
}
