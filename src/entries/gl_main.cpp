#pragma once

#include <glm/glm.hpp>

#include "application_data.hpp"
#include "component_collection.hpp"
#include "components/model.hpp"
#include "systems/window/glfw/window.cpp"
#include "systems/renderer/opengl/model_loader.cpp"
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
	Model teapot = loadModel("teapot.gltf");
	teapot.transform = glm::translate(teapot.transform, glm::vec3(-1.0f, 0.0f, 0.0f));

	// Model suzanne = loadModel("Suzanne.gltf");
	// suzanne.transform = glm::translate(suzanne.transform, glm::vec3(1.0f, 0.0f, 0.0f));

	Model spoon = loadModel("spoon/scene.gltf");
	spoon.transform = glm::translate(spoon.transform, glm::vec3(3.0f, -1.0f, 0.0f));
	spoon.transform = glm::scale(spoon.transform, glm::vec3(0.2f, 0.2f, 0.2f));

	components->models.push_back(teapot);
	components->models.push_back(spoon);

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