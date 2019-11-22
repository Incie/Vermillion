#pragma once

#include"pch.h"
#include"services.h"

class ImageData;

class TextureManagerGL : public TextureService
{
public:
	TextureManagerGL();
	~TextureManagerGL();

	Texture LoadTexture(const std::string& relativePath);
	void UnloadAll();

private:
	void UnloadTexture(Texture& texture);
	unsigned int UploadToGPU(const ImageData& imageData);

	std::vector<Texture*> textures;
};