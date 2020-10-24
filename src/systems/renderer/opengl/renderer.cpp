#pragma once

#include <stdexcept>
#include <string>

#include <GL/glew.h>
#include <tinygltf/tiny_gltf.h>

#include "util/debug.h"
#include "application_data.hpp"

static void loadTeapot() {
	tinygltf::TinyGLTF loader;
	tinygltf::Model model;
	std::string error;
	std::string warning;

	const bool success = loader
		.LoadASCIIFromFile(&model, &error, &warning, "./../assets/teapot.gltf");

	if (!success) {
		if (!error.empty()) {
			LOG(error.c_str())
		}

		if (!warning.empty()) {
			LOG(warning.c_str())
		}

		return;
	}

	for (const tinygltf::Scene &scene : model.scenes) {
		for (const int &nodeIndex : scene.nodes) {
			const tinygltf::Accessor &accessor = model.accessors[nodeIndex];
			const tinygltf::BufferView &bufferView = model.bufferViews[accessor.bufferView];
			const tinygltf::Buffer &buffer = model.buffers[bufferView.buffer];
			
			glBufferData(
				bufferView.target,
				bufferView.byteLength,
				&buffer.data.at(0) + bufferView.byteOffset,
				GL_STATIC_DRAW
			);
		}
	}
}

class OpenGLRenderer {
private:
	ApplicationData *_applicationData;

public:
	OpenGLRenderer(ApplicationData *applicationData) {
		this->_applicationData = applicationData;
	}

	void initialise() const {
		// Init glew 
		const GLenum glewInitResult = glewInit();
		if (glewInitResult != GLEW_OK) {
			this->_applicationData->shouldClose = true;
			throw std::runtime_error(
				"Failed to initialise GLEW. Error code: " + 
				std::to_string(glewInitResult)
			);
			return;
		}

		glViewport(0, 0, 800, 600);
		glClearColor(0.2f, 0.3f, 0.8f, 1.0f);

		// TODO: delete
		loadTeapot();
	}

	void update() const {
		float x = -.5f;
		float y = -.5f;
		float width = 1.0f;
		float height = 1.0f;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBegin(GL_QUADS);
		glVertex3f(x, y, 0.0f);
		glVertex3f(x, y + height, 0.0f);
		glVertex3f(x + width, y + height, 0.0f);
		glVertex3f(x + width, y, 0.0f);
		glEnd();
	}
};