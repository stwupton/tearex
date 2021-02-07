#pragma once

#include <string>

#include "file.cpp"

std::string loadShader(const std::string &fileName) {
	const std::string filePath = std::string(ASSET_PATH) + "shaders/" + fileName;
	return loadFile(filePath);
}