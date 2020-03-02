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

#include "shared_data.hpp"
#include "util/extension_functions.hpp"
#include "../../../util/debug.h"

class RendererInitialiser {
public:
  static VKAPI_ATTR VkBool32 VKAPI_CALL vkDebugLayerCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT *callbackData,
    void *userData
  );

  void run(SharedRendererData *sharedData);

private:
  VkApplicationInfo _createAppInfo() const;

  VkDebugUtilsMessengerCreateInfoEXT _createDebugMessengerCreateInfo() const;

  void _createDebugMessenger(
    VkInstance instance, 
    VkDebugUtilsMessengerEXT *debugMessenger
  );

  void _createDevice();

  void _createInstance(VkInstance *instance);

  int _getFirstQueueFamilyIndex(const VkPhysicalDevice &device) const;

  int _getDeviceScore(const VkPhysicalDevice &device) const;

  std::vector<const char *> _getExtensions() const;

  std::vector<const char *> _getLayers() const;
  
  void _pickPhysicalDevice(
    VkInstance instance, 
    VkPhysicalDevice *physicalDevice
  );
};