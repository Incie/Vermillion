#include"pch.h"
#include "Buffer.h"

Buffer::Buffer(int width, int height)
	: bufferWidth(width), bufferHeight(height)
{
	bufferSize = width * height * 4;
	this->buffer = new char[bufferSize];
}

Buffer::~Buffer()
{
	if (buffer != nullptr)
		delete[]buffer;
	buffer = nullptr;
}

void Buffer::PutPixel(int x, int y, char r, char g, char b)
{
}

void Buffer::RenderBufferToScreen()
{
}
