#pragma once

#include <string>

struct ApplicationData {
	std::string name;
	bool shouldClose = false;
	int windowWidth;
	int windowHeight;
};