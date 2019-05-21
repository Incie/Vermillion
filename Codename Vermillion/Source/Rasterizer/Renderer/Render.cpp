#include"pch.h"
#include "Render.h"

RolfGL::RolfGL()
	: frameBuffer(640,480), c(0)
{
}

RolfGL::~RolfGL()
{
}

void RolfGL::Draw()
{
	c++;
	if (c == 255)
		c = 0;
	frameBuffer.Clear(c, c, c, 1);
	frameBuffer.RenderBufferToScreen();
}

void RolfGL::DrawLine(float x0, float y0, float x1, float y1)
{
	float deltax = x1 - x0;
	float deltay = y1 - y0;

	float deltaErr = 0.0f;
	if (deltax != 0.0f)
		deltaErr = fabs(deltay / deltax);
	float error = 0.0f;

	int sign = 1;
	if (deltay < 0)
		sign = -1;

	int y = (int)y0;
	for (float x = x0; x < x1; x += deltax) {
		frameBuffer.PutPixel((int)x, y, 1, 1, 1);

		error += deltaErr;
		if (error >= 0.5f) {
			y += sign * deltay;
			error -= 1.0f;
		}
	}	
}
