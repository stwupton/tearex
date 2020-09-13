#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

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

#include <util/debug.h>

#include "shared_data.hpp"
#include "util/extension_functions.cpp"

class VulkanInitialiser {
public:
  static VKAPI_ATTR VkBool32 VKAPI_CALL vkDebugLayerCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT *callbackData,
    void *userData
  ) {
    std::cerr << "Validation layer: " << callbackData->pMessage << std::endl;
    return VK_FALSE;
  }

  void run(VulkanSharedData *sharedData) {
    this->_createInstance(&sharedData->instance);
#ifdef DEBUG
    this->_createDebugMessenger(sharedData->instance, &sharedData->debugMessenger);
#endif
    this->_pickPhysicalDevice(sharedData->instance, &sharedData->physicalDevice);
  }

private:
  VkApplicationInfo _createAppInfo() const {
    VkApplicationInfo appInfo {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Hello Triangle";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;
    return appInfo;
  }

  VkDebugUtilsMessengerCreateInfoEXT _createDebugMessengerCreateInfo() const {
    VkDebugUtilsMessengerCreateInfoEXT createInfo {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = 
      VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
      VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
      VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType =
      VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
      VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
      VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = VulkanInitialiser::vkDebugLayerCallback;
    createInfo.pUserData = nullptr;
    return createInfo;
  }

  void _createDebugMessenger(
    VkInstance instance, 
    VkDebugUtilsMessengerEXT *debugMessenger
  ) {
    VkDebugUtilsMessengerCreateInfoEXT messengerCreateInfo = 
      this->_createDebugMessengerCreateInfo();

    VkResult createDebugMessengerResult = 
      ExtensionFunctions::vkCreateDebugUtilsMessengerEXT(
        instance, 
        &messengerCreateInfo, 
        nullptr, 
        debugMessenger
      );

    if (createDebugMessengerResult != VK_SUCCESS) {
      throw std::runtime_error(
        "Failed to create VkDebugUtilsMessengerEXT. Result: " + 
        std::to_string(createDebugMessengerResult)
      );
    }
  }

  void _createDevice() {
    
  }

  void _createInstance(VkInstance *instance) {
    VkApplicationInfo appInfo = this->_createAppInfo();

    VkInstanceCreateInfo createInfo {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    
    std::vector<const char *> extensions = this->_getExtensions();
    createInfo.enabledExtensionCount = extensions.size();
    createInfo.ppEnabledExtensionNames = extensions.data();

    std::vector<const char *> layers = this->_getLayers();
    createInfo.enabledLayerCount = layers.size();
    createInfo.ppEnabledLayerNames = layers.data();

#ifdef DEBUG
    // Specific debug messenger for the creation and destruction of VkInstance
    VkDebugUtilsMessengerCreateInfoEXT instanceMessengerCreateInfo = 
      this->_createDebugMessengerCreateInfo();
    createInfo.pNext = &instanceMessengerCreateInfo;
#else
    createInfo.pNext = nullptr;
#endif

    VkResult createInstanceResult = vkCreateInstance(&createInfo, nullptr, instance);
    if (createInstanceResult != VK_SUCCESS) {
      throw std::runtime_error(
        "Failed to create VkInstance. Result: " + 
        std::to_string(createInstanceResult)
      );
    }
  }

  int _getFirstQueueFamilyIndex(const VkPhysicalDevice &device) const {
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilyProperties(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(
      device, 
      &queueFamilyCount, 
      queueFamilyProperties.data()
    );

    for (int i = 0; i < queueFamilyCount; i++) {
      const VkQueueFamilyProperties qf = queueFamilyProperties[i];
      if (qf.queueFlags & VkQueueFlagBits::VK_QUEUE_GRAPHICS_BIT) {
        return i;
      }
    }

    return -1;
  }

  int _getDeviceScore(const VkPhysicalDevice &device) const {
    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);

    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

    if (!deviceFeatures.geometryShader) {
      return -1;
    }

    int score = deviceProperties.limits.maxImageDimension2D;
    if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
      score += 1000;
    }

    return score;
  }

  std::vector<const char *> _getExtensions() const {
    uint32_t extensionCount = 0;
    const char **requiredExtensions = glfwGetRequiredInstanceExtensions(&extensionCount);
    std::vector<const char *> extensions(
      requiredExtensions, 
      requiredExtensions + extensionCount
    );
#ifdef DEBUG
    extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif
    return extensions;
  }

  std::vector<const char *> _getLayers() const {
#ifdef DEBUG
    std::vector<const char *> validationLayers = {
      "VK_LAYER_KHRONOS_validation"
    };

    uint32_t layerCount = 0;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    // Ensure that the requested validation layers are available
    for (const char *layer : validationLayers) {
      bool hasLayer = false;
      for (const VkLayerProperties &availableLayer : availableLayers) {
        if (strcmp(availableLayer.layerName, layer) == 0) {
          hasLayer = true;
          break;
        }
      }

      if (!hasLayer) {
        throw std::runtime_error(
          "Requested Vulkan validation layer is not available: " + 
          (std::string)layer
        );
      }
    }

    return validationLayers;
#else 
    return std::vector<const char *>(0);
#endif
  }

  void _pickPhysicalDevice(
    VkInstance instance, 
    VkPhysicalDevice *physicalDevice
  ) {
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

    if (deviceCount == 0) {
      throw std::runtime_error("Could not find a GPU that has Vulkan support");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

    int topDeviceScore = -1;
    for (const VkPhysicalDevice &device : devices) {
      int family = this->_getFirstQueueFamilyIndex(device);
      int score = this->_getDeviceScore(device);

      if (family != -1 && score > topDeviceScore) {
        topDeviceScore = score;
        *physicalDevice = device;
      }
    }

    if (*physicalDevice == VK_NULL_HANDLE) {
      throw std::runtime_error("Could not find a suitable GPU");
    }
  }
};