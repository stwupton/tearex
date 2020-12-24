#pragma once

#include <string>

#include "file.cpp"

std::string loadShader(const std::string &fileName) {
	return loadFile("./../assets/shaders/" + fileName);
}