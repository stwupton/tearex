#pragma once

#include <vector>

#include "components/camera.hpp"
#include "components/static_model.hpp"

struct ComponentCollection {
	Camera camera;
	std::vector<StaticModel> staticModels;
};