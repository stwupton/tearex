#pragma once

#include <stdexcept>
#include <string>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "application_data.hpp"
#include "component_collection.hpp"
#include "util/debug.h"
#include "loaders/shader.cpp"
#include "loaders/model_info.hpp"

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
protected:
	ApplicationData *applicationData;
	GLuint basicProgramId;
	ComponentCollection *components;
	ModelInfo *loadedModels;

public:
	OpenGLRenderer(
		ApplicationData *applicationData, 
		ComponentCollection *components,
		ModelInfo* loadedModels
	) : 
		applicationData(applicationData), 
		components(components),
		loadedModels(loadedModels) {}

	void initialise() {
		// Init glew 
		const GLenum glewInitResult = glewInit();
		if (glewInitResult != GLEW_OK) {
			this->applicationData->shouldClose = true;
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

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // TODO: remove

		glClearColor(0.5f, 0.8f, 1.0f, 1.0f);

		// Create shaders
		this->basicProgramId = glCreateProgram();

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
		glAttachShader(this->basicProgramId, vertexShaderId);
		glAttachShader(this->basicProgramId, fragmentShaderId);
		glLinkProgram(this->basicProgramId);

		glDeleteShader(vertexShaderId);
		glDeleteShader(fragmentShaderId);
	}

	void terminate() const {
		glDeleteProgram(this->basicProgramId);
	}

	void update() const {
		const int width = this->applicationData->windowWidth;
		const int height = this->applicationData->windowHeight;

		// If a tree falls in a forest and no one is around to hear it, does it 
		// make a sound?
		if (width == 0 || height == 0) {
			return;
		}

		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glUseProgram(this->basicProgramId);

		const float aspect = (float)width / height;
		const glm::mat4 projection = glm::perspective(45.0f, aspect, 1.0f, 150.0f);
		const glm::mat4 vp = projection * this->components->camera.transform;

		for (const StaticModel &staticModel : this->components->staticModels) {
			const ModelInfo modelInfo = loadedModels[staticModel.modelId];

			const glm::mat4 mvp = vp * staticModel.transform * modelInfo.transform;
			const GLuint mvpUniformLocation = glGetUniformLocation(this->basicProgramId, "mvp");
			glUniformMatrix4fv(mvpUniformLocation, 1, GL_FALSE, &mvp[0][0]);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, modelInfo.textureId);

			glBindVertexArray(modelInfo.vertexArrayId);
			glDrawElements(GL_TRIANGLES, modelInfo.indexLength, modelInfo.indexType, nullptr);
		}
	}
};