#pragma once

#include <stdexcept>
#include <string>

#include <GL/glew.h>

#include "application_data.hpp"

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
	}

	void update() const {
		float x = -0.5f;
		float y = -0.5f;
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