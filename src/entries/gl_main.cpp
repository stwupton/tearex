#pragma once

#include <glm/glm.hpp>

#include "application_data.hpp"
#include "component_collection.hpp"
#include "components/static_model.hpp"
#include "systems/window/glfw/window.cpp"
#include "systems/renderer/opengl/loaders/model_loader.cpp"
#include "systems/renderer/opengl/loaders/model_info.hpp"
#include "systems/renderer/opengl/renderer.cpp"
#include "util/debug.h"

int main() {
	ComponentCollection *components = new ComponentCollection {};
	ApplicationData *applicationData = new ApplicationData {};
	applicationData->name = "OpenGL Window";

	ModelLoader modelLoader;
	GlfwWindow window(applicationData);
	OpenGLRenderer renderer(applicationData, components, modelLoader.loaded);

	window.initialise();
	renderer.initialise();

	// TOOD: remove
	uint8_t teapotModelId = modelLoader.load("teapot.gltf");
	uint8_t spoonModelId = modelLoader.load("spoon/scene.gltf");
	uint8_t suzanneModelId = modelLoader.load("Suzanne.gltf");

	StaticModel teapot { teapotModelId };
	teapot.transform = glm::translate(teapot.transform, glm::vec3(-1.0f, 0.0f, 0.0f));
	teapot.transform = glm::scale(teapot.transform, glm::vec3(0.1f, 0.1f, 0.1f));

	StaticModel suzanne { suzanneModelId };
	suzanne.transform = glm::translate(suzanne.transform, glm::vec3(1.0f, 0.0f, 0.0f));

	StaticModel spoon { spoonModelId };
	spoon.transform = glm::translate(spoon.transform, glm::vec3(3.0f, 0.0f, 0.0f));
	spoon.transform = glm::rotate(spoon.transform, glm::radians(90.0f), glm::vec3(1, 0, 0));
	spoon.transform = glm::scale(spoon.transform, glm::vec3(0.2f, 0.2f, 0.2f));

	components->staticModels.push_back(teapot);
	// components->staticModels.push_back(suzanne);
	components->staticModels.push_back(spoon);

	while (!applicationData->shouldClose) {
		window.update();
		renderer.update();
	}

	window.terminate();
	renderer.terminate();
	modelLoader.unloadAll();

	delete components;
	delete applicationData;

	return EXIT_SUCCESS;
}