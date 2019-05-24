#include"pch.h"
#include "RolfGL.h"

RolfGL::RolfGL()
	: frameBuffer(640,480), c(0)
{
}

RolfGL::~RolfGL()
{
}

void RolfGL::Draw()
{
	c = 0;
	frameBuffer.Clear(c, c, c, 1);

	DrawLine(0,0, 639, 479);
	DrawQuad(150,150, 200,200);
	DrawCircle(100, 300, 50);
	frameBuffer.RenderBufferToScreen();
}

void RolfGL::DrawCircle(float x, float y, float r)
{
	constexpr float PI = 3.1415965f;
	constexpr float THETA = PI * 2.0f;

	for( float phi = 0.0f; phi <= THETA; phi += THETA / 400.0f ){
		float cx = cos(phi) * r;
		float cy = sin(phi) * r;

		frameBuffer.PutPixel((int)x + cx,(int)y + cy, 255,255,255);
	}
}

void RolfGL::DrawQuad(float x0, float y0, float x1, float y1)
{
	for( float x = x0; x <= x1; ++x ){
		for(float y = y0; y <= y1; ++y ){
			frameBuffer.PutPixel((int)x, (int)y, 255, 255, 255);
		}
	}
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
	float x = x0;
	for (float x = x0; x < x1; x += 1.0f) {
		frameBuffer.PutPixel((int)x, y, 255, 255, 255);

		error += deltaErr;
		if (error >= 0.5f) {
			y += (int)sign;
			error -= 1.0f;
		}
	}	
}
