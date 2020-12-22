#pragma once

#include <fstream>
#include <sstream>
#include <string>

// TODO: Make sure that we are not copying the file contents when returning
std::string loadFile(const std::string &path) {
	std::stringstream buffer;
	std::ifstream stream(path);
	buffer << stream.rdbuf();
	return buffer.str();
}