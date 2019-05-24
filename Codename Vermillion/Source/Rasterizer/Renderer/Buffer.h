#pragma once


class Buffer {
public:
	Buffer(int width, int height);
	~Buffer();


	void Clear(unsigned char value);
	void Clear(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	void PutPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b);
	void RenderBufferToScreen();

private:
	void CreateGLTexture();
	void UploadToGPU();

	unsigned char* buffer;
	int bufferSize;
	int bufferWidth;
	int bufferHeight;

	int bufferIdGL;
};