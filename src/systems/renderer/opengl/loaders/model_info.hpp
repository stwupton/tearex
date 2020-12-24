#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

struct ModelInfo {
	// TODO: Hard code a more useful array length for the buffer ids. Should it 
	// even be hardcoded?
	GLuint atrributeBufferIds[4]; 
	GLsizei indexType;
	GLuint indexBufferId; 
	glm::mat4 transform = glm::mat4(1.0f);
	GLuint vertexArrayId;
	GLuint indexLength;
};