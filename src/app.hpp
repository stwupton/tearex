#pragma once

#define GLFW_INCLUDE_VULKAN
#include <glfw/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include <iostream>
#include <stdexcept>
#include <functional>
#include <cstdlib>
#include <vector>
#include <cstring>
#include <string>

#include "util/debug.h"

struct app {
  static VKAPI_ATTR VkBool32 VKAPI_CALL vk_debug_layer_callback(
    VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
    VkDebugUtilsMessageTypeFlagsEXT message_type,
    const VkDebugUtilsMessengerCallbackDataEXT *callback_data,
    void *user_data
  );

  void start();

private:
#ifdef DEBUG
  VkDebugUtilsMessengerEXT _vk_debug_messenger;
#endif
  VkInstance _vk_instance;
  GLFWwindow *_window;

  void _cleanup();
  VkApplicationInfo _create_vk_app_info() const;
  VkDebugUtilsMessengerCreateInfoEXT _create_vk_debug_messenger_create_info() const;
  std::vector<const char *> _get_vk_extensions() const;

  template<typename T>
  T _get_vk_extension_function(const char *function_name) const;

  std::vector<const char *> _get_vk_layers() const;
  void _init_glfw();
  void _init_vk();
  void _loop();
};