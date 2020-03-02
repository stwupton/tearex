#include "terminator.hpp"

void RendererTerminator::run(SharedRendererData *sharedData) {
#ifdef DEBUG
  ExtensionFunctions::vkDestroyDebugUtilsMessengerEXT(
    sharedData->instance,
    sharedData->debugMessenger,
    nullptr
  );
#endif
  vkDestroyInstance(sharedData->instance, nullptr);
}