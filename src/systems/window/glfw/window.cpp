#pragma once

#include <GLFW/glfw3.h>

#include "application_data.hpp"
#include "input_data.hpp"

class GlfwWindow {
protected:
	ApplicationData *applicationData;
	InputData *inputData;
	GLFWwindow *window;

protected:
	void updateInput() {
		int state = glfwGetMouseButton(this->window, GLFW_MOUSE_BUTTON_LEFT);
		this->inputData->primaryMouseDown = state == GLFW_PRESS;

		this->inputData->previousMouseX = this->inputData->mouseX;
		this->inputData->previousMouseY = this->inputData->mouseY;

		glfwGetCursorPos(
			this->window, 
			&this->inputData->mouseX, 
			&this->inputData->mouseY
		);
	}

public:
	GlfwWindow(ApplicationData *sharedSystemData, InputData *inputData) {
		this->applicationData = sharedSystemData;
		this->inputData = inputData;
	}

	void initialise() {
		glfwInit();

		const char *windowName = this->applicationData->name.c_str();
		this->window = glfwCreateWindow(800, 600, windowName, NULL, NULL);

		glfwMakeContextCurrent(this->window);
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	}

	void terminate() {
		glfwDestroyWindow(this->window);
		glfwTerminate();
	}

	void update() {
		glfwPollEvents();
		this->updateInput();

		glfwSwapInterval(2);
		glfwSwapBuffers(this->window);

		bool shouldClose = glfwWindowShouldClose(this->window);
		this->applicationData->shouldClose = shouldClose;

		glfwGetWindowSize(
			this->window, 
			&this->applicationData->windowWidth, 
			&this->applicationData->windowHeight
		);
	}
};