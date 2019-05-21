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
