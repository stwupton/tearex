cmake_minimum_required(VERSION 3.15.3)

set(CMAKE_CXX_VERSION 17)

link_directories(
  $ENV{VULKAN_SDK}/Lib
  ${CMAKE_SOURCE_DIR}/libs
)

include_directories(
  $ENV{VULKAN_SDK}/Include
  ${CMAKE_SOURCE_DIR}/inc
)

file(GLOB_RECURSE SRC_FILES . src/*.cpp)

project(tearex)
add_executable(tearex main.cpp ${SRC_FILES})
target_link_libraries(tearex vulkan-1 glfw3)