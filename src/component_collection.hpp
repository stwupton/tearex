#pragma once

#include <vector>

#include "components/camera.hpp"
#include "components/directional_light.hpp"
#include "components/static_model.hpp"

struct ComponentCollection {
	Camera camera;
	std::vector<StaticModel> staticModels;
	// TODO: Change to vector to support multiple spot lights
	DirectionalLight directionalLight;
};