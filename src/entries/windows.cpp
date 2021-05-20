#include <Windows.h>
#include <GL/glew.h>
#include <GL/wglew.h>

#include "input_data.hpp"
#include "systems/renderer/opengl/renderer.cpp"
#include "systems/renderer/opengl/loaders/model_loader.cpp"
#include "systems/window/win32/message_handler.cpp"
#include "types.hpp"

INT createWin32Window(HINSTANCE instanceHandle, INT showFlag) {
	const LPCSTR className = "OpenGL Example Window";
	const HBRUSH backgroundBrush = CreateSolidBrush(RGB(150, 150, 150));

	WNDCLASS windowClass = {};
	windowClass.hbrBackground = backgroundBrush;
	windowClass.lpfnWndProc = handle;
	windowClass.hInstance = instanceHandle;
	windowClass.lpszClassName = className;
	windowClass.style = CS_OWNDC;
	RegisterClass(&windowClass);

	HWND windowHandle = CreateWindowEx(
		0,
		className,
		"OpenGL Window",
		WS_OVERLAPPEDWINDOW,
		0,
		200,
		applicationData->windowWidth,
		applicationData->windowHeight,
		NULL,
		NULL,
		instanceHandle,
		NULL
	);

	if (windowHandle == NULL) {
		return 0;
	}

	ShowWindow(windowHandle, showFlag);

	return 0;
}

INT WINAPI wWinMain(
	HINSTANCE instanceHandle, 
	HINSTANCE prevInstanceHandle, 
	PWSTR cmdArgs, 
	INT showFlag
) {
	applicationData = new ApplicationData {};
	applicationData->name = "OpenGL Window";
	applicationData->windowWidth = 800;
	applicationData->windowHeight = 600;

	inputData = new InputData {};

	createWin32Window(instanceHandle, showFlag);

	ComponentCollection *components = new ComponentCollection {};
	ModelLoader modelLoader;

	OpenGLRenderer renderer(applicationData, components, modelLoader.loaded);
	renderer.initialise();

	if (WGLEW_EXT_swap_control) {
		wglSwapIntervalEXT(1);
	} else {
		// TODO(steven): Do something else if there is no swap interval extension
	}

	// TODO(steven): Debug stuff. Remove
	{
		u8 suzanneModelId = modelLoader.load("Suzanne.gltf");
		StaticModel suzanne { suzanneModelId };
		components->staticModels.push_back(suzanne);

		// u8 teapotModelId = modelLoader.load("teapot.gltf");
		// StaticModel teapot { teapotModelId };
		// teapot.transform = glm::translate(teapot.transform, glm::vec3(0.0f, 0.0f, 0.0f));
		// teapot.transform = glm::scale(teapot.transform, glm::vec3(0.05f, 0.05f, 0.05f));
		// components->staticModels.push_back(teapot);

		// u8 spoonModelId = modelLoader.load("spoon/scene.gltf");
		// StaticModel spoon { spoonModelId };
		// spoon.transform = glm::rotate(spoon.transform, glm::radians(90.0f), glm::vec3(1, 0, 0));
		// spoon.transform = glm::scale(spoon.transform, glm::vec3(0.1f));
		// components->staticModels.push_back(spoon);

		components->directionalLight.direction = glm::vec3(-1, 0, 0);
		components->directionalLight.colour = glm::vec3(1);

		Camera &camera = components->camera;
		camera.transform = glm::translate(camera.transform, glm::vec3(0.0f, 0.0f, -5.0f));
	}

	MSG message = {};
	while (!applicationData->shouldClose) {
		while (PeekMessage(&message, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&message);
			DispatchMessage(&message);
		}

		// TODO(steven): Move somewhere more suitable
		{
			POINT cursorPosition;
			GetCursorPos(&cursorPosition);
			inputData->previousMouseX = inputData->mouseX;
			inputData->previousMouseY = inputData->mouseY;
			inputData->mouseX = cursorPosition.x;
			inputData->mouseY = cursorPosition.y;
		}

		// TODO: Remove. Basic logic for moving object in view
		{
			static glm::vec2 rotation;

			StaticModel &model = components->staticModels[0];
			const float sensitivity = 0.4f;
			if (inputData->primaryMouseDown) {
				rotation = glm::vec2(
					glm::radians((float)inputData->mouseYMovement()), 
					glm::radians((float)inputData->mouseXMovement())
				) * sensitivity;

				char buffer[30] = {};
				sprintf(buffer, "%f\n", inputData->mouseYMovement());
				OutputDebugString(buffer);
			} else {
				rotation *= 0.9f;
			}

			if (glm::length(rotation) > 0) {
				const glm::vec2 rotationAxis = glm::normalize(rotation);
				const glm::mat4 rotationMatrix = glm::rotate(
					glm::mat4(1.0f), 
					glm::length(rotation), 
					glm::vec3(rotationAxis.x, rotationAxis.y, 0.0f)
				);
				model.transform = rotationMatrix * model.transform;
			}
		}

		renderer.update();

		SwapBuffers(deviceContext);
	}
}