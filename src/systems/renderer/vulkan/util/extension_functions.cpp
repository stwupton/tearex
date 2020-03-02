#include "extension_functions.hpp"

VkResult ExtensionFunctions::vkCreateDebugUtilsMessengerEXT(
  VkInstance vkInstance,
  VkDebugUtilsMessengerCreateInfoEXT *vkMessengerCreateInfo,
  const VkAllocationCallbacks *allocator,
  VkDebugUtilsMessengerEXT *vkDebugMessenger
) {
  return ExtensionFunctions::getVkExtensionFunction<PFN_vkCreateDebugUtilsMessengerEXT>(
    vkInstance,
    "vkCreateDebugUtilsMessengerEXT"
  )(vkInstance, vkMessengerCreateInfo, allocator, vkDebugMessenger);
}

void ExtensionFunctions::vkDestroyDebugUtilsMessengerEXT(
  VkInstance vkInstance, 
  VkDebugUtilsMessengerEXT vkDebugMessenger,
  const VkAllocationCallbacks *allocator
) {
  ExtensionFunctions::getVkExtensionFunction<PFN_vkDestroyDebugUtilsMessengerEXT>(
    vkInstance,
    "vkDestroyDebugUtilsMessengerEXT"
  )(vkInstance, vkDebugMessenger, allocator);
}

template<typename T>
T ExtensionFunctions::getVkExtensionFunction(
  VkInstance vkInstance, 
  const char *functionName
) {
  T extensionFunction = (T)vkGetInstanceProcAddr(vkInstance, functionName);
  if (extensionFunction == nullptr) {
    throw std::runtime_error(
      "Failed to get extension function: " + 
      std::string(functionName)
    );
  }
  return extensionFunction;
}