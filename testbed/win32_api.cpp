// NOTE(steven): Compile with...
// clang-cl testbed/win32_api.cpp /o build/win32_api /link user32.lib

#include <Windows.h>
#include <cstdint>
#include <stdio.h>

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float f32;
typedef double f64;

LRESULT CALLBACK windowMessageHandler(
	HWND handle, 
	UINT message, 
	WPARAM wParam, 
	LPARAM lParam
) {
	LRESULT result;

	switch (message) {
		case WM_DESTROY: {
			PostQuitMessage(0);
			result = 0;
			break;
		}

		case WM_PAINT: {
			PAINTSTRUCT paintStruct;
			HDC hdc = BeginPaint(handle, &paintStruct);

			FillRect(hdc, &paintStruct.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
			EndPaint(handle, &paintStruct);

			result = 0;
			break;
		}

		default: {
			result = DefWindowProc(handle, message, wParam, lParam);
		}
	}

	return result;
}

INT WINAPI wWinMain(
	HINSTANCE hInstance, 
	HINSTANCE hPrevInstance, 
	PWSTR pCmdLine, 
	INT nCmdShow
) {
	const char className[] = "Example Window Class";

	WNDCLASS windowClass = {};
	windowClass.lpfnWndProc = windowMessageHandler;
	windowClass.hInstance = hInstance;
	windowClass.lpszClassName = className;
	RegisterClass(&windowClass);

	HWND handle = CreateWindowEx(
		0,
		className,
		"Example Window",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
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