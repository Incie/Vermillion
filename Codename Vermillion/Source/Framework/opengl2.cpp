#include "opengl2.h"
#include<gl/glu.h>

GL2Renderer::GL2Renderer()
{
	hWnd = nullptr;
	hDC = nullptr;
	hRC = nullptr;
}

void GL2Renderer::SetHandles(HWND hWnd)
{
	this->hWnd = hWnd;
	this->hDC = GetDC(hWnd);
}

void GL2Renderer::CreateRenderContext()
{
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 32;
	pfd.iLayerType = PFD_MAIN_PLANE;

	int nPixelFormat = ChoosePixelFormat(hDC, &pfd);

	if (nPixelFormat == 0)
		return;

	BOOL result = SetPixelFormat(hDC, nPixelFormat, &pfd);

	hRC = wglCreateContext(hDC);
	wglMakeCurrent(hDC, hRC);
}

void GL2Renderer::DestroyRenderContext()
{
	wglMakeCurrent(NULL, NULL);
	if(hRC != nullptr) {
		wglDeleteContext(hRC);
	}

	if(hDC != nullptr) {
		ReleaseDC(hWnd, hDC);
		hWnd = nullptr;
		hDC = nullptr;
	}
}


void GL2Renderer::SetViewport(int width, int height)
{
	if (height == 0)
		return;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glViewport(0, 0, width, height);
	glOrtho(0, width, 0, height, -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void GL2Renderer::StartFrame()
{
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
}

void GL2Renderer::EndFrame()
{
	SwapBuffers(hDC);
}
