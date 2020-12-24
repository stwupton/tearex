#pragma once

#include <cstdint>

#include <glm/glm.hpp>

struct StaticModel {
	uint8_t modelId;
	glm::mat4 transform = glm::mat4(1.0f);
};