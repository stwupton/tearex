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

add_executable(tearex ${CMAKE_SOURCE_DIR}/src/main.cpp)
add_compile_definitions(${RENDERER} ${WINDOW_MANAGER})

if(RENDERER STREQUAL "VULKAN")
  set(RENDERER_LIB vulkan-1)
elseif(RENDERER STREQUAL "OPEN_GL")
  add_compile_definitions(GLEW_STATIC)
  set(RENDERER_LIB glew32s opengl32)
endif()

if(WINDOW_MANAGER STREQUAL "GLFW3")
  add_compile_definitions(GLFW_INCLUDE_NONE)
  set(WINDOW_MANAGER_LIB glfw3)
endif()

target_link_libraries(tearex ${RENDERER_LIB} ${WINDOW_MANAGER_LIB})