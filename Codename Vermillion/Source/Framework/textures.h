#pragma once

#include<string>
#include<vector>

class ImageData;

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
	void UnloadTexture(Texture& texture);
	unsigned int UploadToGPU(const ImageData& imageData);

	std::vector<Texture*> textures;
};