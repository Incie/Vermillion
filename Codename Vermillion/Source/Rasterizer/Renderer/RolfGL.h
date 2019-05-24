#pragma once

#include"Buffer.h"

class RolfGL 
{
public:
	RolfGL();
	~RolfGL();

	void Draw();
	void DrawCircle(float x, float y, float r);
	void DrawLine(float x0, float y0, float x1, float y1);
	void DrawQuad(float x0, float y0, float x1, float y1);

private:
	Buffer frameBuffer;
	char c;
};