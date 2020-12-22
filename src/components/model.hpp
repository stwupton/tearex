#pragma once

#include <glm/glm.hpp>

struct Model {
	// TODO: Hard code a more useful array length for the buffer ids. Should it 
	// even be hardcoded?
	uint32_t bufferIds[4]; 
	int indexType;
	uint32_t indexBufferId; 
	uint32_t vertexArrayId;
	uint32_t vertexLength;

	// TODO: remove this, this shouldn't be here
	glm::mat4 transform = glm::mat4(1.0f);
};