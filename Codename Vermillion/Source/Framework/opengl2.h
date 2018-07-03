#pragma once

#include<Windows.h>


class GL2Renderer
{
public:
	GL2Renderer();

	void SetHandles(HWND hWnd);

	void CreateRenderContext();
	void DestroyRenderContext();


	void SetViewport(int width, int height);
	void StartFrame();
	void EndFrame();


private:
	HWND hWnd;
	HDC hDC;
	HGLRC hRC;
};