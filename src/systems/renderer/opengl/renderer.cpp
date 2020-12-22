#pragma once

#include <stdexcept>
#include <string>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "application_data.hpp"
#include "component_collection.hpp"
#include "util/debug.h"
#include "shader_loader.cpp"

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
	GLuint _basicProgramId;
	ComponentCollection *_components;

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

		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback(handleGLError, nullptr);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glViewport(0, 0, 800, 600);
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // TODO: remove

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
		// TODO: Add an unload to the model loader when we have one
		for (const Model &model : this->_components->models) {
			glDeleteVertexArrays(1, &model.vertexArrayId);
			glDeleteBuffers(sizeof(model.bufferIds) / sizeof(uint32_t), model.bufferIds);
		}
	}

	void update() const {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glUseProgram(this->_basicProgramId);

		float aspect = (float)
			this->_applicationData->windowWidth / 
			this->_applicationData->windowHeight;
		glm::mat4 projection = glm::perspective(45.0f, aspect, 1.0f, 150.0f);
		glm::mat4 view = glm::mat4(1.0f);
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -8.0f));
		glm::mat4 mvp = projection * view;

		const GLuint mvpUniformLocation = glGetUniformLocation(this->_basicProgramId, "mvp");
		glUniformMatrix4fv(mvpUniformLocation, 1, GL_FALSE, &mvp[0][0]);

		for (const Model &model : this->_components->models) {
			glBindVertexArray(model.vertexArrayId);
			glDrawElements(GL_TRIANGLES, model.vertexLength, GL_UNSIGNED_SHORT, nullptr);
		}
	}
};