#pragma once

#include <glm/glm.hpp>

struct DirectionalLight {
  glm::vec3 colour = glm::vec3(1);
  glm::vec3 direction = glm::vec3(0, -1, 0);
};