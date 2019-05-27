#include"pch.h"
#include "RolfGL.h"

#include"glm/gtc/matrix_transform.hpp"

RolfGL::RolfGL()
	: 
	projectionMatrix(glm::perspectiveFov(90.0f, 640.0f, 480.0f, 1.0f, 1000.0f)),
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

	DrawLine(t.vertices[0].x, t.vertices[0].y, t.vertices[1].x, t.vertices[1].y);
	DrawLine(t.vertices[1].x, t.vertices[1].y, t.vertices[2].x, t.vertices[2].y);
	DrawLine(t.vertices[0].x, t.vertices[0].y, t.vertices[2].x, t.vertices[2].y);
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
