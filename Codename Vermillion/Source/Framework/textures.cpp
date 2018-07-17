#include"textures.h"
#include"textures_pngloader.h"

#include"GL\glew.h"

TextureManager::TextureManager()
{
}

TextureManager::~TextureManager()
{
}

Texture TextureManager::LoadTexture(const std::string & relativePath)
{
	auto loader = new PNGLoader();
	auto imageLoadStatus = loader->Read(relativePath);

	if (imageLoadStatus != ImageLoaderStatus::LOADER_SUCCESS) {
		return Texture();
	}

	auto imageData = loader->Get();

	GLint format = GL_RGB;
	if (imageData.channels == 4)
		format = GL_RGBA;

	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		format,
		imageData.width,
		imageData.height,
		0,
		format,
		GL_UNSIGNED_BYTE,
		imageData.data
	);

	// Set texture options
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return Texture{texture, imageData.width, imageData.height, imageData.channels};
}/*
 	int textureId;
	int width;
	int height;
	int channels;
 */

void TextureManager::UnloadAll()
{
}
