#pragma once

#include <vulkan/vulkan.h>

#include "shared_data.hpp"
#include "util/extension_functions.cpp"

class VulkanTerminator {
public:
  void run(VulkanSharedData *sharedData) {
#ifdef DEBUG
    ExtensionFunctions::vkDestroyDebugUtilsMessengerEXT(
      sharedData->instance,
      sharedData->debugMessenger,
      nullptr
    );
#endif
    vkDestroyInstance(sharedData->instance, nullptr);
  }
};