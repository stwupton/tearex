cmake_minimum_required(VERSION 3.15.3)
set(CMAKE_CXX_STANDARD 17)

project(tearex)

link_directories(
  $ENV{VULKAN_SDK}/Lib
  ${CMAKE_SOURCE_DIR}/lib
)

include_directories(
  ${CMAKE_SOURCE_DIR}/src
  $ENV{VULKAN_SDK}/Include
  ${CMAKE_SOURCE_DIR}/include
)

add_executable(tearex main.cpp)
add_compile_definitions(${RENDERER} ${WINDOW_MANAGER})

if(RENDERER STREQUAL "VULKAN")
  find_library(RENDERER_LIB vulkan vulkan-1)
endif()

if(WINDOW_MANAGER STREQUAL "GLFW3")
  find_library(WINDOW_MANAGER_LIB glfw glfw3)
endif()

target_link_libraries(tearex ${RENDERER_LIB} ${WINDOW_MANAGER_LIB})