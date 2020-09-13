#pragma once

#include <GLFW/glfw3.h>

#include <application_data.hpp>

class GlfwWindow {
private:
  ApplicationData *_sharedSystemData;
  GLFWwindow *_window;

public:
  GlfwWindow(ApplicationData *sharedSystemData) {
    this->_sharedSystemData = sharedSystemData;
  }

  void initialise() {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    this->_window = glfwCreateWindow(800, 600, "Vulkan window", nullptr, nullptr);
  }

  void terminate() {
    glfwDestroyWindow(this->_window);
    glfwTerminate();
  }

  void update() {
    glfwPollEvents();

    bool shouldClose = glfwWindowShouldClose(this->_window);
    this->_sharedSystemData->applicationShouldClose = shouldClose;
  }
};