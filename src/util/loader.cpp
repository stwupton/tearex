#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

// TODO: Make sure that we are not copying the file contents when returning
std::string load(const std::string &path) {
	std::stringstream buffer;
	std::ifstream stream(path);
	buffer << stream.rdbuf();
	return buffer.str();
}

std::string loadShader(const std::string &fileName) {
	return load("./../assets/shaders/" + fileName);
}

std::string loadModel(const std::string &fileName) {
	return load("./../assets/models/" + fileName);
}