#pragma once

#include <stdexcept>
#include <string>

#include <GL/glew.h>

#include "application_data.hpp"
#include "component_collection.hpp"
#include "util/debug.h"
#include "loader.cpp"

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
	ComponentCollection *_components;

	// TODO: delete
	GLuint _bufferId;
	GLuint _basicProgramId;

public:
	OpenGLRenderer(
		ApplicationData *applicationData, 
		ComponentCollection *components
	) : 
		_applicationData(applicationData), 
		_components(components) {}

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
		const char *fragmentShaderSource = fragmentShader.c_str();

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
		glUseProgram(this->_basicProgramId);

		for (const Model &model : this->_components->models) {
			for (const uint32_t &bufferId : model.bufferIds) {
				glBindBuffer(GL_ARRAY_BUFFER, bufferId);
			}
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model.indexBufferId);
			glDrawElements(GL_TRIANGLES, model.vertexLength, GL_UNSIGNED_INT, nullptr);
		}
	}
};