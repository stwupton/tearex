cmake_minimum_required(VERSION 3.15.3)
set(CMAKE_CXX_STANDARD 17)

project(TEAREX)

set(
	CMAKE_LIBRARY_PATH
	$ENV{VULKAN_SDK}/Lib
	${CMAKE_SOURCE_DIR}/lib
)

string(REPLACE ";" " " LINK_DIRECTORIES ${CMAKE_LIBRARY_PATH})
link_directories(${LINK_DIRECTORIES})

include_directories(
	${CMAKE_SOURCE_DIR}/src
	$ENV{VULKAN_SDK}/Include
	${CMAKE_SOURCE_DIR}/include
	${CMAKE_SOURCE_DIR}/include/glm
)

add_executable(TEAREX ${CMAKE_SOURCE_DIR}/src/main.cpp)
add_compile_definitions(${RENDERER} ${WINDOW_MANAGER})

if(RENDERER STREQUAL "VULKAN")
	find_library(RENDERER_LIB NAMES vulkan-1 vulkan)
elseif(RENDERER STREQUAL "OPEN_GL")
	add_compile_definitions(
		GLEW_STATIC 

		# Definitions needed for tinygltf
		TINYGLTF_IMPLEMENTATION 
		STB_IMAGE_IMPLEMENTATION 
		STB_IMAGE_WRITE_IMPLEMENTATION
	)
	find_library(GLEW_LIB NAMES glew glew32s GLEW)
	find_package(OpenGL REQUIRED)
	set(RENDERER_LIB ${GLEW_LIB} ${OPENGL_gl_LIBRARY})
endif()

if(WINDOW_MANAGER STREQUAL "GLFW3")
	add_compile_definitions(GLFW_INCLUDE_NONE)
	find_library(WINDOW_MANAGER_LIB NAMES glfw3 glfw)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	add_compile_definitions(ASSET_PATH="./../assets/")
else()
	add_compile_definitions(ASSET_PATH="./assets/")
endif()

target_link_libraries(TEAREX ${RENDERER_LIB} ${WINDOW_MANAGER_LIB})