#pragma once

#include <GLFW/glfw3.h>

#include <application_data.hpp>

class GlfwWindow {
private:
  ApplicationData *_applicationData;
  GLFWwindow *_window;

public:
  GlfwWindow(ApplicationData *sharedSystemData) {
    this->_applicationData = sharedSystemData;
  }

  void initialise() {
    glfwInit();

    const char *windowName = this->_applicationData->name.c_str();
    this->_window = glfwCreateWindow(800, 600, windowName, nullptr, nullptr);

    glfwMakeContextCurrent(this->_window);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  }

  void terminate() {
    glfwDestroyWindow(this->_window);
    glfwTerminate();
  }

  void update() {
    glfwPollEvents();
    glfwSwapInterval(2);
    glfwSwapBuffers(this->_window);

    bool shouldClose = glfwWindowShouldClose(this->_window);
    this->_applicationData->shouldClose = shouldClose;
  }
};