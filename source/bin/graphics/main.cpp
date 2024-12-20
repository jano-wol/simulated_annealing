#include "GLFWInit.h"
#include "GLFWStop.h"
#include "LoopStart.h"
#include "FileBrowser.h"
#include "Render.h"
#include "SAFactory.h"
#include "Scale.h"
#include "StateUI.h"
#include "WindowBegin.h"
#include "WindowEnd.h"

using namespace sa::io;

int main()
{
  GLFWwindow* window = GLFWInit("SA");
  scale();
  StateUI state;
  while (!glfwWindowShouldClose(window)) {
    loopStart();
    windowBegin();
    state.menu();
    saFactoryUpdate(state);
    windowEnd();
    render(window);
  }
  GLFWStop(window);
  return 0;
}
