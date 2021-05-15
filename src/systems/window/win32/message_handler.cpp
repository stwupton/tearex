// NOTE(steven): File must be included before wWinMain

#include <Windows.h>

#include "GL/wglew.h"

#include "application_data.hpp"
#include "input_data.hpp"

// TODO(steven): temporary 
static ApplicationData *applicationData;
static InputData *inputData;
static HDC deviceContext;
static HGLRC glRenderingContext;

LRESULT CALLBACK handle(
	HWND windowHandle, 
	UINT message, 
	WPARAM wParam, 
	LPARAM lParam
) {
	LRESULT result = 0;

	switch (message) {
		case WM_CREATE: {
			PIXELFORMATDESCRIPTOR pixelFormatDescriptor = { 
				sizeof(PIXELFORMATDESCRIPTOR),  //  size of this pfd  
				1,                              // version number  
				PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, 
				PFD_TYPE_RGBA,
				24,                             // 24-bit color depth  
				0, 0, 0, 0, 0, 0,               // color bits ignored  
				0,                              // no alpha buffer  
				0,                              // shift bit ignored  
				0,                              // no accumulation buffer  
				0, 0, 0, 0,                     // accum bits ignored  
				32,                             // 32-bit z-buffer      
				0,                              // no stencil buffer  
				0,                              // no auxiliary buffer  
				PFD_MAIN_PLANE,                 // main layer  
				0,                              // reserved  
				0, 0, 0                         // layer masks ignored  
			}; 

			deviceContext = GetDC(windowHandle);
			const int pixelFormat = ChoosePixelFormat(deviceContext, &pixelFormatDescriptor);
			SetPixelFormat(deviceContext, pixelFormat, &pixelFormatDescriptor);

			glRenderingContext = wglCreateContext(deviceContext);
			wglMakeCurrent(deviceContext, glRenderingContext);

			// TODO(steven): Do something else if there is no swap interval extension
			PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
			wglSwapIntervalEXT(2);
		} break;

		case WM_CLOSE: {
			applicationData->shouldClose = true;
			DestroyWindow(windowHandle);
		} break;

		case WM_DESTROY: {
			ReleaseDC(windowHandle, deviceContext);
			wglDeleteContext(glRenderingContext);
			PostQuitMessage(0);
		} break;

		case WM_PAINT: {
			PAINTSTRUCT paintStruct;
			HDC hdc = BeginPaint(windowHandle, &paintStruct);
			
			// TODO(steven): 

			EndPaint(windowHandle, &paintStruct);
		} break;

		case WM_SIZE: {
			applicationData->windowWidth = LOWORD(lParam);
			applicationData->windowHeight = HIWORD(lParam);
		} break;

		case WM_LBUTTONDOWN: {
			inputData->primaryMouseDown = true;
		} break;

		case WM_LBUTTONUP: {
			inputData->primaryMouseDown = false;
		} break;

		default: {
			result = DefWindowProc(windowHandle, message, wParam, lParam);
		}
	}

	return result;
}