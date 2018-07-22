#pragma once

class Texture {
public:
	unsigned int textureId;
	int width;
	int height;
	unsigned int channels;
};

class TextureService {
public:
	virtual Texture LoadTexture(const std::string& relativePath) = 0;
	virtual void UnloadAll() = 0;
};


class TextService {
	virtual void Print(double x, double y, std::string &text) = 0;
};