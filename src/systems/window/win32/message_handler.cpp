// NOTE(steven): File must be included before wWinMain

#include <Windows.h>
#include <stdio.h>

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
		} break;

		case WM_PAINT: {
			PAINTSTRUCT paintStruct;
			HDC hdc = BeginPaint(handle, &paintStruct);
			
			// TODO(steven): 

			EndPaint(handle, &paintStruct);

			result = 0;
		} break;

		default: {
			result = DefWindowProc(handle, message, wParam, lParam);
		}
	}

	return result;
}