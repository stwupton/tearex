#include <iostream>

#include "util/debug.h"

#if VULKAN && GLFW3
#include "entries/vk_main.cpp"
#elif OPEN_GL && GLFW3
#include "entries/gl_main.cpp"
#else
#error Could not find entry point for current variant.
#endif