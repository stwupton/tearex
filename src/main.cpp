#include <iostream>

#include "util/debug.h"

#if defined VULKAN && defined GLFW3
#include "entries/vk_main.cpp"
#elif defined OPEN_GL && defined GLFW3
#include "entries/gl_main.cpp"
#else
#error Could not find entry point for current variant.
#endif
