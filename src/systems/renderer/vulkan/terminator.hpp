#pragma once

#include <vulkan/vulkan.h>

#include "shared_data.hpp"
#include "util/extension_functions.hpp"

class RendererTerminator {
public:
  void run(SharedRendererData *sharedData);
};