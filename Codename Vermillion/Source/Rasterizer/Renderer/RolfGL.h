#pragma once

#include"glm/glm.hpp"
#include"../Primitives/Primitives.h"
#include"Buffer.h"

class RolfGL 
{
public:
	RolfGL();
	~RolfGL();

	void StartFrame();
	void EndFrame();

	void DrawTriangle(const Triangle& triangle);

	void Draw();
	void DrawCircle(float x, float y, float r);
	void DrawLine(float x0, float y0, float x1, float y1);
	void DrawQuad(float x0, float y0, float x1, float y1);

private:
	glm::mat4 projectionMatrix;
	Buffer frameBuffer;
	char c;
};