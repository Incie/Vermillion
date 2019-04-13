#include"pch.h"

#include"textures_pngloader.h"
#include"png.h"
#include"pnglibconf.h"
#include"log.h"

//todo: logtag macro?
//automaticly use logtag in a LOG macro?
static const char* const LOGTAG = "PNGLoader";

PNGLoader::PNGLoader()
{
}

PNGLoader::~PNGLoader()
{
}

bool ReadPNGInfo(FILE* fp, png_structp& png_ptr, png_infop& info_ptr, ImageData& imageData) {

	if (setjmp(png_jmpbuf(png_ptr)))
		return false;

	png_init_io(png_ptr, fp);
	png_set_sig_bytes(png_ptr, 8);

	png_read_info(png_ptr, info_ptr);

	imageData.width = png_get_image_width(png_ptr, info_ptr);
	imageData.height = png_get_image_height(png_ptr, info_ptr);
	imageData.colordepth = png_get_bit_depth(png_ptr, info_ptr);
	imageData.channels = png_get_channels(png_ptr, info_ptr);

	auto color_type = png_get_color_type(png_ptr, info_ptr);

	int number_of_passes = png_set_interlace_handling(png_ptr);
	png_read_update_info(png_ptr, info_ptr);
	return true;
}

bool ReadImage(png_structp& png_ptr, png_infop& info_ptr, ImageData& imageData) {
	/* read file */
	if (setjmp(png_jmpbuf(png_ptr)))
		return false;

	auto size = sizeof(png_bytep) * imageData.height;
	auto stride = png_get_rowbytes(png_ptr, info_ptr);

	imageData.data = new unsigned char[size*stride];

	png_bytep* row_pointers = new png_bytep[imageData.height];
	for (int y = 0; y < imageData.height; y++)
		row_pointers[y] = &imageData.data[y * stride];

	png_read_image(png_ptr, row_pointers);

	delete[] row_pointers;

	return true;
}

ImageLoaderStatus PNGLoader::Read(const std::string& filepath)
{
	/* open file and test for it being a png */
	FILE *fp = NULL;
	fopen_s(&fp, filepath.c_str(), "rb");

	if (!fp) {
		Log::Error(LOGTAG, fmt::format("Could not open file {0}", filepath));
		return ImageLoaderStatus::FILE_NOT_FOUND;
	}
	
	unsigned char header[8];    // 8 is the maximum size that can be checked
	fread(header, 1, 8, fp);
	if (png_sig_cmp(header, 0, 8)) {
		Log::Error(LOGTAG, fmt::format("Png Headercheck: Image ({0}) is not a valid PNG", filepath));
		return ImageLoaderStatus::INVALID_IMAGE;
	}
	
	/* initialize stuff */
	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png_ptr) {
		Log::Error(LOGTAG, fmt::format("Failed during png_create_read_struct on file: {0}", filepath));
		return ImageLoaderStatus::UNDEFINED;
	}

	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr) {
		Log::Error(LOGTAG, fmt::format("Failed during png_create_info_struct on file: {0}", filepath));
		return ImageLoaderStatus::UNDEFINED;
	}

	ReadPNGInfo(fp, png_ptr, info_ptr, imageData);
	ReadImage(png_ptr, info_ptr, imageData);
	png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)0);
	fclose(fp);
	return ImageLoaderStatus::LOADER_SUCCESS;
}


void PNGLoader::Free()
{
	if (imageData.data != nullptr)
		delete[]imageData.data;
	imageData.data = nullptr;
}

const ImageData & PNGLoader::Get() const
{
	return imageData;
}