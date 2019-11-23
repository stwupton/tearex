#include "app.hpp"

VKAPI_ATTR VkBool32 VKAPI_CALL app::vk_debug_layer_callback(
  VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
  VkDebugUtilsMessageTypeFlagsEXT message_type,
  const VkDebugUtilsMessengerCallbackDataEXT *callback_data,
  void *user_data
) {
  std::cerr << "Validation layer: " << callback_data->pMessage << std::endl;
  return VK_FALSE;
}

void app::start() {
  TRY("INIT GLFW", this->_init_glfw(), NULL)
  TRY("INIT VULKAN", this->_init_vk(), NULL)
  TRY("APPLICATION LOOP", this->_loop(), NULL)
  TRY("APPLICATION CLEANUP", this->_cleanup(), NULL)
}

VkApplicationInfo app::_create_vk_app_info() const {
  VkApplicationInfo app_info {};
  app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  app_info.pApplicationName = "Hello Triangle";
  app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  app_info.pEngineName = "No Engine";
  app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  app_info.apiVersion = VK_API_VERSION_1_0;
  return app_info;
}

VkDebugUtilsMessengerCreateInfoEXT app::_create_vk_debug_messenger_create_info() const {
  VkDebugUtilsMessengerCreateInfoEXT create_info {};
  create_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
  create_info.messageSeverity = 
    VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
    VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
    VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
  create_info.messageType =
    VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
    VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
    VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
  create_info.pfnUserCallback = app::vk_debug_layer_callback;
  create_info.pUserData = nullptr;
  return create_info;
}

void app::_cleanup() {
#ifdef DEBUG 
  this->_get_vk_extension_function<PFN_vkDestroyDebugUtilsMessengerEXT>(
    "vkDestroyDebugUtilsMessengerEXT"
  )(this->_vk_instance, this->_vk_debug_messenger, nullptr);
#endif
  vkDestroyInstance(this->_vk_instance, nullptr);

  glfwDestroyWindow(this->_window);
  glfwTerminate();
}

std::vector<const char *> app::_get_vk_extensions() const {
  uint32_t extension_count = 0;
  const char **required_extensions = glfwGetRequiredInstanceExtensions(&extension_count);
  std::vector<const char *> extensions(
    required_extensions, 
    required_extensions + extension_count
  );
#ifdef DEBUG
  extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif
  return extensions;
}

std::vector<const char *> app::_get_vk_layers() const {
#ifdef DEBUG
  std::vector<const char *> validation_layers = {
    "VK_LAYER_KHRONOS_validation"
  };

  uint32_t layer_count = 0;
  vkEnumerateInstanceLayerProperties(&layer_count, nullptr);

  std::vector<VkLayerProperties> available_layers(layer_count);
  vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data());

  // Ensure that the requested validation layers are available
  for (const char *layer : validation_layers) {
    bool has_layer = false;

    for (const VkLayerProperties &available_layer : available_layers) {
      if (strcmp(available_layer.layerName, layer) == 0) {
        has_layer = true;
        break;
      }
    }

    if (!has_layer) {
      throw std::runtime_error("Requested Vulkan validation layers are not available");
    }
  }

  return validation_layers;
#else 
  return std::vector<const char *>(0);
#endif
}

template<typename T>
T app::_get_vk_extension_function(const char *function_name) const {
  T extension_function = (T)vkGetInstanceProcAddr(this->_vk_instance, function_name);
  if (extension_function == nullptr) {
    throw std::runtime_error(
      "Failed to get extension function: " + 
      std::string(function_name)
    );
  }
  return extension_function;
}

void app::_init_glfw() {
  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  this->_window = glfwCreateWindow(800, 600, "Vulkan window", nullptr, nullptr);
}

void app::_init_vk() {
  VkApplicationInfo app_info = this->_create_vk_app_info();

  VkInstanceCreateInfo create_info {};
  create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  create_info.pApplicationInfo = &app_info;
  
  std::vector<const char *> extensions = this->_get_vk_extensions();
  create_info.enabledExtensionCount = extensions.size();
  create_info.ppEnabledExtensionNames = extensions.data();

  std::vector<const char *> layers = this->_get_vk_layers();
  create_info.enabledLayerCount = layers.size();
  create_info.ppEnabledLayerNames = layers.data();

#ifdef DEBUG
  // Specific debug messenger for the creation and destruction of VkInstance
  VkDebugUtilsMessengerCreateInfoEXT instance_messenger_create_info = 
    this->_create_vk_debug_messenger_create_info();
  create_info.pNext = &instance_messenger_create_info;
#else
  create_info.pNext = nullptr;
#endif

  VkResult create_instance_result = vkCreateInstance(&create_info, nullptr, &this->_vk_instance);
  if (create_instance_result != VK_SUCCESS) {
    throw std::runtime_error(
      "Failed to create VkInstance. Result: " + 
      std::to_string(create_instance_result)
    );
  }

#ifdef DEBUG
  VkDebugUtilsMessengerCreateInfoEXT messenger_create_info = 
    this->_create_vk_debug_messenger_create_info();

  VkResult create_debug_messenger_result = 
    this->_get_vk_extension_function<PFN_vkCreateDebugUtilsMessengerEXT>(
      "vkCreateDebugUtilsMessengerEXT"
    )(this->_vk_instance, &messenger_create_info, nullptr, &this->_vk_debug_messenger);

  if (create_debug_messenger_result != VK_SUCCESS) {
    throw std::runtime_error(
      "Failed to create VkDebugUtilsMessengerEXT. Result: " + 
      std::to_string(create_debug_messenger_result)
    );
  }
#endif
}

void app::_loop() {
  while (!glfwWindowShouldClose(this->_window)) {
    glfwPollEvents();
  }
}