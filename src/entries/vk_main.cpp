#pragma once

#include <application_data.hpp>
#include <systems/renderer/vulkan/renderer.cpp>
#include <systems/window/glfw/window.cpp>
#include <util/debug.h>

int main() {
  ApplicationData *applicationData = new ApplicationData {};

  GlfwWindow window(applicationData);
  VulkanRenderer renderer;

  window.initialise();
  renderer.initialise();

  while (!applicationData->applicationShouldClose) {
    window.update();
  }

  renderer.terminate();
  window.terminate();

  delete applicationData;

  return EXIT_SUCCESS;
}