#pragma once


class Buffer {
public:
	Buffer(int width, int height);
	~Buffer();

	void PutPixel(int x, int y, char r, char g, char b);
	void RenderBufferToScreen();

private:
	char* buffer;
	int bufferSize;
	int bufferWidth;
	int bufferHeight;
};