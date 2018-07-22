#pragma once

#include<string>
#include<vector>

#include"services.h"

class ImageData;

class TextureManager : TextureService
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