#pragma once

#include <glfw/glfw3.h>

#include "shared_data.hpp"
#include "../../shared_data.hpp"

class WindowInitialiser {
public:
  void run(SharedWindowData *sharedData, SharedSystemData *sharedSystemData);
};