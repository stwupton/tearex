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
	) {
		return ExtensionFunctions::getVkExtensionFunction<PFN_vkCreateDebugUtilsMessengerEXT>(
			vkInstance,
			"vkCreateDebugUtilsMessengerEXT"
		)(vkInstance, vkMessengerCreateInfo, allocator, vkDebugMessenger);
	}

	static void vkDestroyDebugUtilsMessengerEXT(
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
	static T getVkExtensionFunction(
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
};