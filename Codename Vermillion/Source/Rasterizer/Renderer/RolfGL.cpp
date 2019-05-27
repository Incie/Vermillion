#include"pch.h"
#include "RolfGL.h"

#include"glm/gtc/matrix_transform.hpp"

RolfGL::RolfGL()
	: 
	projectionMatrix(glm::perspectiveFov(3.1415f*0.5f, 640.0f, 480.0f, 0.1f, 1000.0f)),
	frameBuffer(640,480), 
	c(0)
{
}

RolfGL::~RolfGL()
{
}

void RolfGL::StartFrame()
{
	frameBuffer.Clear(25, 25, 100, 255);
}

void RolfGL::EndFrame()
{
	frameBuffer.RenderBufferToScreen();
}

void RolfGL::DrawTriangle(const Triangle& triangle)
{
	Triangle t{triangle};
	t.Transform(projectionMatrix);

	for( auto& v : t.vertices ){
		v.x = (v.x + 1.0f) * 0.5f * 640.0f;
		v.y = (v.y + 1.0f) * 0.5f * 480.0f;
	}

	DL(t.vertices[0].x, t.vertices[0].y, t.vertices[1].x, t.vertices[1].y);
	DL(t.vertices[1].x, t.vertices[1].y, t.vertices[2].x, t.vertices[2].y);
	DL(t.vertices[2].x, t.vertices[2].y, t.vertices[0].x, t.vertices[0].y);
}

void RolfGL::Draw()
{
	DrawLine(0,0, 639, 479);
	DrawQuad(150,150, 200,200);
	DrawCircle(100, 300, 50);
	
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
	if (x0 > x1) {
		float t = x0;
		x0 = x1;
		x1 = t;
	}


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

void RolfGL::DL(float x2, float y2, float x1, float y1)
{
	int x, y, dx, dy, dx1, dy1, px, py, xe, ye, i;
	dx = x2 - x1; dy = y2 - y1;

	// straight lines idea by gurkanctn
	if (dx == 0) // Line is vertical
	{
		if (y2 < y1) std::swap(y1, y2);
		for (y = y1; y <= y2; y++)
			frameBuffer.PutPixel(x1, y, 255, 255, 255);
		return;
	}

	if (dy == 0) // Line is horizontal
	{
		if (x2 < x1) std::swap(x1, x2);
		for (x = x1; x <= x2; x++)
			frameBuffer.PutPixel(x, y1, 255, 255, 255);
		return;
	}

	// Line is Funk-aye
	dx1 = abs(dx); dy1 = abs(dy);
	px = 2 * dy1 - dx1;	py = 2 * dx1 - dy1;
	if (dy1 <= dx1)
	{
		if (dx >= 0)
		{
			x = x1; y = y1; xe = x2;
		}
		else
		{
			x = x2; y = y2; xe = x1;
		}

		frameBuffer.PutPixel(x, y, 255, 255, 255);

		for (i = 0; x < xe; i++)
		{
			x = x + 1;
			if (px < 0)
				px = px + 2 * dy1;
			else
			{
				if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) y = y + 1; else y = y - 1;
				px = px + 2 * (dy1 - dx1);
			}
			frameBuffer.PutPixel(x, y, 255, 255, 255);
		}
	}
	else
	{
		if (dy >= 0)
		{
			x = x1; y = y1; ye = y2;
		}
		else
		{
			x = x2; y = y2; ye = y1;
		}

		frameBuffer.PutPixel(x, y, 255, 255, 255);

		for (i = 0; y < ye; i++)
		{
			y = y + 1;
			if (py <= 0)
				py = py + 2 * dx1;
			else
			{
				if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) x = x + 1; else x = x - 1;
				py = py + 2 * (dx1 - dy1);
			}

			frameBuffer.PutPixel(x, y, 255, 255, 255);
		}
	}
}
