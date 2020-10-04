#pragma once

#include "application_data.hpp"
#include "systems/window/glfw/window.cpp"
#include "systems/renderer/opengl/renderer.cpp"
#include "util/debug.h"

int main() {
	ApplicationData *applicationData = new ApplicationData {};
	applicationData->name = "OpenGL Window";

	GlfwWindow window(applicationData);
	OpenGLRenderer renderer(applicationData);

	window.initialise();
	renderer.initialise();

	while (!applicationData->shouldClose) {
		window.update();
		renderer.update();
	}

	window.terminate();

	delete applicationData;

	return EXIT_SUCCESS;
}