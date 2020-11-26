#pragma once

#include <stdexcept>
#include <string>

#include <GL/glew.h>
#include <tinygltf/tiny_gltf.h>

#include "util/debug.h"
#include "util/loader.cpp"
#include "application_data.hpp"

static void handleGLError(
	GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar *message,
	const void *userParam
) {
	LOG(source << ", " << type << ", " << id << ", " << message)
}

class OpenGLRenderer {
private:
	ApplicationData *_applicationData;

	// TODO: delete
	GLuint _bufferId;
	GLuint _basicProgramId;

public:
	OpenGLRenderer(ApplicationData *applicationData) {
		this->_applicationData = applicationData;
	}

	void initialise() {
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
		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback(handleGLError, nullptr);

		// TODO: delete following
		tinygltf::TinyGLTF loader;
		tinygltf::Model model;
		std::string error;
		std::string warning;

		const std::string teapot = loadModel("teapot.gltf");
		const char *teapotSource = teapot.c_str();
		const bool success = loader.LoadASCIIFromString(
			&model, 
			&error, 
			&warning, 
			teapotSource,
			strlen(teapotSource),
			"./../assets/models/"
		);

		if (!warning.empty()) {
			LOG(warning.c_str())
		}

		if (!success) {
			if (!error.empty()) {
				LOG(error.c_str())
			} 
			return;
		}

		for (const tinygltf::Scene &scene : model.scenes) {
			for (const int &nodeIndex : scene.nodes) {
				const tinygltf::Accessor &accessor = model.accessors[nodeIndex];
				const tinygltf::BufferView &bufferView = model.bufferViews[accessor.bufferView];
				const tinygltf::Buffer &buffer = model.buffers[bufferView.buffer];

				glGenBuffers(1, &this->_bufferId);
				glBindBuffer(bufferView.target, this->_bufferId);
				glBufferData(
					bufferView.target,
					bufferView.byteLength,
					&buffer.data.at(0) + bufferView.byteOffset,
					GL_STATIC_DRAW
				);
				glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, sizeof(float), 0);
				glEnableVertexAttribArray(0);
			}
		}

		// Create shaders
		this->_basicProgramId = glCreateProgram();

		// Vertex shader
		std::string vertexShader = loadShader("basic.vert");
		const char *vertexShaderSource = vertexShader.c_str();

		GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShaderId, 1, &vertexShaderSource, nullptr);
		glCompileShader(vertexShaderId);

		// Fragment shader
		std::string fragmentShader = loadShader("basic.frag");
		const char *fragmentShaderSource = vertexShader.c_str();

		GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShaderId, 1, &fragmentShaderSource, nullptr);
		glCompileShader(fragmentShaderId);

		// Attach program
		glAttachShader(this->_basicProgramId, vertexShaderId);
		glAttachShader(this->_basicProgramId, fragmentShaderId);
		glLinkProgram(this->_basicProgramId);

		glDeleteShader(vertexShaderId);
		glDeleteShader(fragmentShaderId);
	}

	void terminate() const {
		glDeleteProgram(this->_basicProgramId);
		glDeleteBuffers(1, &this->_bufferId);
	}

	void update() const {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBindBuffer(GL_ARRAY_BUFFER, this->_bufferId);
		glUseProgram(this->_basicProgramId);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}
};