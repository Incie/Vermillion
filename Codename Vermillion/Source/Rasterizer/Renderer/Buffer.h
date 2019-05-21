#pragma once


class Buffer {
public:
	Buffer(int width, int height);
	~Buffer();


	void Clear(char value);
	void Clear(char r, char g, char b, char a);
	void PutPixel(int x, int y, char r, char g, char b);
	void RenderBufferToScreen();

private:
	void CreateGLTexture();
	void UploadToGPU();

	char* buffer;
	int bufferSize;
	int bufferWidth;
	int bufferHeight;

	int bufferIdGL;
};