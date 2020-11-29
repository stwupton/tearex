#pragma once

#include "application_data.hpp"
#include "component_collection.hpp"
#include "systems/window/glfw/window.cpp"
#include "systems/renderer/opengl/renderer.cpp"
#include "util/debug.h"

int main() {
	ComponentCollection *components = new ComponentCollection {};
	ApplicationData *applicationData = new ApplicationData {};
	applicationData->name = "OpenGL Window";

	GlfwWindow window(applicationData);
	OpenGLRenderer renderer(applicationData, components);

	window.initialise();
	renderer.initialise();

	// TOOD: remove
	components->models.push_back(loadModel("teapot.gltf"));

	while (!applicationData->shouldClose) {
		window.update();
		renderer.update();
	}

	window.terminate();
	renderer.terminate();

	delete components;
	delete applicationData;

	return EXIT_SUCCESS;
}