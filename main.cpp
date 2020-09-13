#include <iostream>
#include <util/debug.h>

#if defined VULKAN && defined GLFW3
#include <entries/vk_main.cpp>
#else
#error Could not find entry point for current variant.
#endif
