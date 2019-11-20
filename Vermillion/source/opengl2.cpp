#include"pch.h"

#include "opengl2.h"
#include"GL/glew.h"
#include<gl/glu.h>
#include"log.h"

void CheckOpenGLErrors(const char* statement, const char* sourceFileName, int lineNumber)
{
	GLenum errorCode = 0;
	while ((errorCode = glGetError()) != GL_NO_ERROR) {
		const char* errorEnum = nullptr;
		switch (errorCode) {
			case GL_INVALID_ENUM: errorEnum = "GL_INVALID_ENUM"; break;
			case GL_INVALID_VALUE: errorEnum = "GL_INVALID_VALUE"; break;
			case GL_INVALID_OPERATION: errorEnum = "GL_INVALID_OPERATION"; break;
			case GL_INVALID_FRAMEBUFFER_OPERATION: errorEnum = "GL_INVALID_FRAMEBUFFER_OPERATION"; break;
			case GL_OUT_OF_MEMORY: errorEnum = "GL_OUT_OF_MEMORY"; break;
			case GL_STACK_UNDERFLOW: errorEnum = "GL_STACK_UNDERFLOW"; break;
			case GL_STACK_OVERFLOW: errorEnum = "GL_STACK_OVERFLOW"; break;
			default: errorEnum = "unknown errorCode"; break;
		}


		Log::Error("GLError", fmt::format("OpenGL Error Code {}({}) [{}, {}, {}]", errorCode, statement, sourceFileName, lineNumber));
	}
}

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
	Log::Info("OpenGL2", "Creating renderingcontext");

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

	if (nPixelFormat == 0) {
		Log::Error("OpenGL2", "Could not find PixelFormat");
		return;
	}

	if (!SetPixelFormat(hDC, nPixelFormat, &pfd)) {
		Log::Error("OpenGL2", "Could not find Set PixelFormat");
		return;
	}

	hRC = wglCreateContext(hDC);
	if (hRC == NULL) {
		Log::Error("OpenGL2", "Could not find Create Rendering Context");
		return;
	}

	if (!wglMakeCurrent(hDC, hRC)) {
		Log::Error("OpenGL2", "Could not make Rendering Context Current");
		return;
	}

	auto glewInitResult = glewInit();
	if (glewInitResult != GLEW_OK) {
		Log::Error("OpenGL2", fmt::format("GL Extension Wrangler failed to initialize with code: {0}", glewInitResult) );
	}

	Log::Info("OpenGL2", "Rendercontext created");
}

void GL2Renderer::DestroyRenderContext()
{
	Log::Info("OpenGL2", fmt::format("DestroyRenderContext {}", __FUNCTION__));
	wglMakeCurrent(NULL, NULL);

	if (hRC != nullptr) {
		Log::Info("OpenGL2", "Deleting Rendering Context");
		wglDeleteContext(hRC);
	}

	if (hDC != nullptr) {
		Log::Info("OpenGL2", "Releasing DC");
		ReleaseDC(hWnd, hDC);
		hWnd = nullptr;
		hDC = nullptr;
	}
}


void GL2Renderer::SetViewport(int width, int height)
{
	if (height == 0) {
		Log::Error("OpenGL2", "SetViewPort -> height == 0");
		return;
	}

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glViewport(0, 0, width, height);
	glOrtho(0, width, height, 0, -100, 100);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void GL2Renderer::StartFrame()
{
	GLCHECK(glClearColor(0, 0, 0, 0))
	GLCHECK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT))
	GLCHECK(glLoadIdentity())
}

void GL2Renderer::EndFrame()
{
	GLCHECK(SwapBuffers(hDC))
}
