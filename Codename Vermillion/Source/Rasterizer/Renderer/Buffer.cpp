#include"pch.h"
#include"Buffer.h"
#include"GL/glew.h"

Buffer::Buffer(int width, int height)
	: bufferWidth(width), bufferHeight(height), bufferIdGL(0)
{
	bufferSize = width * height * 4;
	buffer = new char[bufferSize];

	Clear(0, 0, 0, 1);
	CreateGLTexture();
}

Buffer::~Buffer()
{
	if (buffer != nullptr)
		delete[] buffer;
	buffer = nullptr;
}

void Buffer::Clear(char value)
{
	Clear(value, value, value, value);
}

void Buffer::Clear(char r, char g, char b, char a)
{
	for (int i = 0; i < bufferSize; i += 4) {
		buffer[i + 0] = r;
		buffer[i + 1] = g;
		buffer[i + 2] = b;
		buffer[i + 3] = a;
	}
}

void Buffer::PutPixel(int x, int y, char r, char g, char b)
{
	if (x < bufferWidth && x >= 0 && y >= 0 && y < bufferHeight) {
		int pixelIndex = (x + y * bufferWidth) * 4;
		buffer[pixelIndex + 0] = r;
		buffer[pixelIndex + 1] = g;
		buffer[pixelIndex + 2] = b;
		buffer[pixelIndex + 3] = 255;
	}
}

void Buffer::RenderBufferToScreen()
{
	glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, bufferIdGL);
		UploadToGPU();
	glDisable(GL_TEXTURE_2D);
}

void Buffer::CreateGLTexture()
{
	GLint format = GL_RGBA;
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, (GLuint*) &bufferIdGL);

	glBindTexture(GL_TEXTURE_2D, bufferIdGL);
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		format,
		bufferWidth,
		bufferHeight,
		0,
		format,
		GL_UNSIGNED_BYTE,
		buffer
	);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	glDisable(GL_TEXTURE_2D);
}

void Buffer::UploadToGPU()
{
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, bufferWidth, bufferHeight, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
}
