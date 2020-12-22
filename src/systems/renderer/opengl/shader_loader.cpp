#pragma once

#include <string>

#include "file_loader.cpp"

std::string loadShader(const std::string &fileName) {
	return loadFile("./../assets/shaders/" + fileName);
}