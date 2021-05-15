#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include "application_data.hpp"
#include "component_collection.hpp"
#include "components/camera.hpp"
#include "components/static_model.hpp"
#include "systems/window/glfw/window.cpp"
#include "systems/renderer/opengl/loaders/model_loader.cpp"
#include "systems/renderer/opengl/loaders/model_info.hpp"
#include "systems/renderer/opengl/renderer.cpp"
#include "util/debug.h"

int main() {
	ComponentCollection *components = new ComponentCollection {};
	InputData *inputData = new InputData {};
	ApplicationData *applicationData = new ApplicationData {};
	applicationData->name = "OpenGL Window";

	ModelLoader modelLoader;
	GlfwWindow window(applicationData, inputData);
	OpenGLRenderer renderer(applicationData, components, modelLoader.loaded);

	window.initialise();
	renderer.initialise();

	// TODO: remove
	{
		uint8_t suzanneModelId = modelLoader.load("Suzanne.gltf");
		StaticModel suzanne { suzanneModelId };
		components->staticModels.push_back(suzanne);

		// uint8_t teapotModelId = modelLoader.load("teapot.gltf");
		// StaticModel teapot { teapotModelId };
		// teapot.transform = glm::translate(teapot.transform, glm::vec3(0.0f, 0.0f, 0.0f));
		// teapot.transform = glm::scale(teapot.transform, glm::vec3(0.05f, 0.05f, 0.05f));
		// components->staticModels.push_back(teapot);

		// uint8_t spoonModelId = modelLoader.load("spoon/scene.gltf");
		// StaticModel spoon { spoonModelId };
		// spoon.transform = glm::rotate(spoon.transform, glm::radians(90.0f), glm::vec3(1, 0, 0));
		// components->staticModels.push_back(spoon);

		// components->directionalLight.direction = glm::vec3(-1, 0, 0);
		// components->directionalLight.colour = glm::vec3(1);

		// Camera &camera = components->camera;
		// camera.transform = glm::translate(camera.transform, glm::vec3(0.0f, 0.0f, -25.0f));
	}

	// TODO: Remove
	while (!applicationData->shouldClose) {
		window.update();

		// TODO: Remove. Basic logic for moving object in view
		{
			static glm::vec2 rotation;

			StaticModel &model = components->staticModels[0];
			const float sensitivity = 0.4f;
			if (inputData->primaryMouseDown) {
				rotation = glm::vec2(
					glm::radians((float)inputData->mouseYMovement()), 
					glm::radians((float)inputData->mouseXMovement())
				) * sensitivity;
			} else {
				rotation *= 0.9f;
			}

			if (glm::length(rotation) > 0) {
				const glm::vec2 rotationAxis = glm::normalize(rotation);
				const glm::mat4 rotationMatrix = glm::rotate(
					glm::mat4(1.0f), 
					glm::length(rotation), 
					glm::vec3(rotationAxis.x, rotationAxis.y, 0.0f)
				);
				model.transform = rotationMatrix * model.transform;
			}
		}

		renderer.update();
	}

	window.terminate();
	renderer.terminate();
	modelLoader.unloadAll();

	return EXIT_SUCCESS;
}