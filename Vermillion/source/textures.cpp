#include"pch.h"
#include"textures.h"
#include"textures_pngloader.h"

#include"GL\glew.h"

TextureManagerGL::TextureManagerGL()
{
}

TextureManagerGL::~TextureManagerGL()
{
}

Texture TextureManagerGL::LoadTexture(const std::string & relativePath)
{
	Log::Info("TextureManagerGL", fmt::format("Loading texture {}", relativePath));

	auto loader = vnew PNGLoader();
	auto imageLoadStatus = loader->Read(relativePath);

	if (imageLoadStatus != ImageLoaderStatus::LOADER_SUCCESS) {
		return Texture();
	}

	auto imageData = loader->Get();

	auto texture = UploadToGPU(imageData);

	loader->Free();
	delete loader;

	return Texture{texture, imageData.width, imageData.height, imageData.channels};
}

void TextureManagerGL::UnloadAll()
{
	for (auto texture : textures) {
		UnloadTexture(*texture);
	}

	textures.clear();
}

void TextureManagerGL::UnloadTexture(Texture & texture)
{
	GLCHECK(glDeleteTextures(1, &texture.textureId))
	texture.textureId = 0;
}

unsigned int TextureManagerGL::UploadToGPU(const ImageData& imageData)
{
	GLint format = GL_RGB;
	if (imageData.channels == 4)
		format = GL_RGBA;

	GLuint textureId;
	GLCHECK(glGenTextures(1, &textureId));
	GLCHECK(glBindTexture(GL_TEXTURE_2D, textureId));
	GLCHECK(glTexImage2D(
		GL_TEXTURE_2D,
		0,
		format,
		imageData.width,
		imageData.height,
		0,
		format,
		GL_UNSIGNED_BYTE,
		imageData.data
	));

	// Set texture options
	GLCHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	GLCHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

	return textureId;
}
