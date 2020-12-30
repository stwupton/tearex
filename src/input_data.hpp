#pragma once

struct InputData {
	double mouseX;
	double mouseY;
	double previousMouseX;
	double previousMouseY;
	bool primaryMouseDown;

	double mouseXMovement() const {
		return this->mouseX - this->previousMouseX;
	}

	double mouseYMovement() const {
		return this->mouseY - this->previousMouseY;
	}
};