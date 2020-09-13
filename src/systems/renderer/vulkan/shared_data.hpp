#pragma once

#include <vulkan/vulkan.h>

#include <util/debug.h>

struct VulkanSharedData {
#ifdef DEBUG
  VkDebugUtilsMessengerEXT debugMessenger;
#endif
  VkDevice device;
  VkInstance instance;
  VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
};
