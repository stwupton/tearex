#include "updater.hpp"

void WindowUpdater::run(
  SharedWindowData *sharedData, 
  SharedSystemData *sharedSystemData
) {
  glfwPollEvents();
  
  bool shouldClose = glfwWindowShouldClose(sharedData->window);
  sharedSystemData->applicationShouldClose = shouldClose;
}