#pragma once

#include<Windows.h>

//todo: rename, or expand?
//interface with a rendererbase?
class GL2Renderer
{
public:
	GL2Renderer();
	
	//todo: howto remove this?
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