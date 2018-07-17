#pragma once

#include<string>

struct ImageData
{
	int width;
	int height;
	int colordepth;
	unsigned int channels;
	unsigned char* data;
};

enum ImageLoaderStatus {
	FILE_NOT_FOUND,
	INVALID_IMAGE,
	LOADER_SUCCESS,
	UNDEFINED
};


class PNGLoader
{
public:
	PNGLoader();
	~PNGLoader();

	ImageLoaderStatus Read(const std::string& file);
	void Free();

	const ImageData& Get() const;

private:
	ImageData imageData;
};