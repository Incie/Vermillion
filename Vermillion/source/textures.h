#pragma once

#include"services.h"
#include<memory>

class ImageData;

class TextureManagerGL : public TextureService
{
public:
	TextureManagerGL();
	~TextureManagerGL();

	std::shared_ptr<Texture> LoadTexture(const FilePath& relativePath);
	void UnloadAll();

private:
	void UnloadTexture(Texture& texture);
	unsigned int UploadToGPU(const ImageData& imageData);
	std::shared_ptr<Texture> FindExisting(const FilePath& relativePath);

	std::vector<std::shared_ptr<Texture>> textures;
};