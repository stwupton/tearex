#include "terminator.hpp"

void WindowTerminator::run(
  SharedWindowData *sharedData, 
  SharedSystemData *sharedSystemData
) {
  glfwDestroyWindow(sharedData->window);
  glfwTerminate();
}