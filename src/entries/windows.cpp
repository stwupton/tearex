#include <Windows.h>

#include "input_data.hpp"
#include "systems/renderer/opengl/renderer.cpp"
#include "systems/renderer/opengl/loaders/model_loader.cpp"
#include "systems/window/win32/message_handler.cpp"

INT createWin32Window(HINSTANCE hInstance, INT nCmdShow) {
	const LPCSTR className = "Example Window Class";
	const HBRUSH backgroundBrush = CreateSolidBrush(RGB(150, 150, 150));

	WNDCLASS windowClass = {};
	windowClass.hbrBackground = backgroundBrush;
	windowClass.lpfnWndProc = windowMessageHandler;
	windowClass.hInstance = hInstance;
	windowClass.lpszClassName = className;
	RegisterClass(&windowClass);

	HWND handle = CreateWindowEx(
		0,
		className,
		"Example Window",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	if (handle == NULL) {
		return 0;
	}

	ShowWindow(handle, nCmdShow);

	MSG message = {};
	while (GetMessage(&message, NULL, 0, 0)) {
		TranslateMessage(&message);
		DispatchMessage(&message);
	}

	return 0;
}

INT WINAPI wWinMain(
	HINSTANCE hInstance, 
	HINSTANCE hPrevInstance, 
	PWSTR pCmdLine, 
	INT nCmdShow
) {
	ComponentCollection *components = new ComponentCollection {};
	InputData *inputData = new InputData {};
	ApplicationData *applicationData = new ApplicationData {};
	applicationData->name = "OpenGL Window";

	ModelLoader modelLoader;
	OpenGLRenderer renderer(applicationData, components, modelLoader.loaded);

	createWin32Window(hInstance, nCmdShow);
}