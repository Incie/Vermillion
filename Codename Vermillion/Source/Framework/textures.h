#pragma once

#include<string>

class Texture {
public:
	unsigned int textureId;
	int width;
	int height;
	unsigned int channels;
};

class TextureManager
{
public:
	TextureManager();
	~TextureManager();

	Texture LoadTexture(const std::string& relativePath);

	void UnloadAll();

private:

};