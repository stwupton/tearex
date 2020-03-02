#pragma once

#include <vulkan/vulkan.h>

#include <stdexcept>

class ExtensionFunctions {
public:
  static VkResult vkCreateDebugUtilsMessengerEXT(
    VkInstance vkInstance,
    VkDebugUtilsMessengerCreateInfoEXT *vkMessengerCreateInfo,
    const VkAllocationCallbacks *allocator,
    VkDebugUtilsMessengerEXT *vkDebugMessenger
  );

  static void vkDestroyDebugUtilsMessengerEXT(
    VkInstance vkInstance, 
    VkDebugUtilsMessengerEXT vkDebugMessenger,
    const VkAllocationCallbacks *allocator
  );

  template<typename T>
  static T getVkExtensionFunction(
    VkInstance vkInstance, 
    const char *functionName
  );
};