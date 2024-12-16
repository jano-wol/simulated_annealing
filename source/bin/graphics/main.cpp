#include "GLFWInit.h"
#include "GLFWStop.h"
#include "LoopStart.h"
#include "Menu.h"
#include "Render.h"
#include "Scale.h"
#include "UIState.h"
#include "WindowBegin.h"
#include "WindowEnd.h"

using namespace sa::io;

int main()
{
  GLFWwindow* window = GLFWInit("SA");
  scale();
  UIState state;
  while (!glfwWindowShouldClose(window)) {
    loopStart();
    windowBegin();
    menu(state);
    windowEnd();
    render(window);
  }
  GLFWStop(window);
  return 0;
}
