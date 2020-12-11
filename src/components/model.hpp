#pragma once

struct Model {
	// TODO: Hard code a more useful array length for the buffer ids. Should it 
	// even be hardcoded?
	uint32_t bufferIds[3]; 
	uint32_t vertexArrayId;
	uint32_t vertexLength;
};