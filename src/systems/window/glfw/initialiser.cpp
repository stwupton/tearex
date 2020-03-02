#include "initialiser.hpp"

void WindowInitialiser::run(
  SharedWindowData *sharedData, 
  SharedSystemData *sharedSystemData
) {
  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  sharedData->window = glfwCreateWindow(800, 600, "Vulkan window", nullptr, nullptr);
}