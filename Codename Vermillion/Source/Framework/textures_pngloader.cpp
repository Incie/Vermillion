#include "textures_pngloader.h"
#include"libpng16/png.h"
#include"pnglibconf.h"

PNGLoader::PNGLoader()
{
}

PNGLoader::~PNGLoader()
{
}

ImageLoaderStatus PNGLoader::Read(const std::string& filepath)
{
	unsigned char header[8];    // 8 is the maximum size that can be checked
	int x, y;

	int width, height;
	png_byte color_type;
	png_byte bit_depth;

	png_structp png_ptr;
	png_infop info_ptr;
	int number_of_passes;
	png_bytep * row_pointers;
	
	/* open file and test for it being a png */
	FILE *fp;
	fopen_s(&fp, filepath.c_str(), "rb");
	fread(header, 1, 8, fp);
	if (png_sig_cmp(header, 0, 8))
		return ImageLoaderStatus::UNDEFINED;

	/* initialize stuff */
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

	if (!png_ptr)
		return ImageLoaderStatus::UNDEFINED;

	info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr)
		return ImageLoaderStatus::UNDEFINED;

	if (setjmp(png_jmpbuf(png_ptr)))
		return ImageLoaderStatus::UNDEFINED;

	png_init_io(png_ptr, fp);
	png_set_sig_bytes(png_ptr, 8);

	png_read_info(png_ptr, info_ptr);

	width = png_get_image_width(png_ptr, info_ptr);
	height = png_get_image_height(png_ptr, info_ptr);
	color_type = png_get_color_type(png_ptr, info_ptr);
	bit_depth = png_get_bit_depth(png_ptr, info_ptr);
	png_uint_32 channels = png_get_channels(png_ptr, info_ptr);

	number_of_passes = png_set_interlace_handling(png_ptr);
	png_read_update_info(png_ptr, info_ptr);


	/* read file */
	if (setjmp(png_jmpbuf(png_ptr)))
		return ImageLoaderStatus::UNDEFINED;

	auto size = sizeof(png_bytep) * height;
	auto stride = png_get_rowbytes(png_ptr, info_ptr);

	auto data = new unsigned char[size*stride];
	row_pointers = new png_bytep[height];
	for (y = 0; y < height; y++)
		row_pointers[y] = &data[y * stride];

	png_read_image(png_ptr, row_pointers);

	imageData = ImageData{ width,height, color_type, channels, data };

	fclose(fp);
	return ImageLoaderStatus::LOADER_SUCCESS;

	//unsigned char header[8];    // 8 is the maximum size that can be checked
	//int x, y;

	//png_structp png_ptr;
	//png_infop info_ptr;
	//int number_of_passes;

	///* open file and test for it being a png */
	//FILE *fp = nullptr;
	//fopen_s(&fp, filepath.c_str(), "rb");
	//if (!fp)
	//	return ImageLoaderStatus::FILE_NOT_FOUND;

	//fread(header, 1, 8, fp);
	//if (png_sig_cmp(header, 0, 8))
	//	return ImageLoaderStatus::INVALID_IMAGE;


	///* initialize stuff */
	//png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

	//if (!png_ptr)
	//	return ImageLoaderStatus::UNDEFINED;
	//	//abort_("[read_png_file] png_create_read_struct failed");

	//info_ptr = png_create_info_struct(png_ptr);
	//if (!info_ptr)
	//	//abort_("[read_png_file] png_create_info_struct failed");
	//	return ImageLoaderStatus::UNDEFINED;

	//if (setjmp(png_jmpbuf(png_ptr)))
	//	//abort_("[read_png_file] Error during init_io");
	//	return ImageLoaderStatus::UNDEFINED;

	//png_init_io(png_ptr, fp);
	//png_set_sig_bytes(png_ptr, 8);

	//png_read_info(png_ptr, info_ptr);

	//png_byte color_type;
	//png_byte bits_per_channel;

	//imageData.width = png_get_image_width(png_ptr, info_ptr);
	//imageData.height = png_get_image_height(png_ptr, info_ptr);
	//color_type = png_get_color_type(png_ptr, info_ptr);
	//bits_per_channel = png_get_bit_depth(png_ptr, info_ptr);
	//png_uint_32 channels = png_get_channels(png_ptr, info_ptr);

	//imageData.colordepth = bits_per_channel;

	//switch (color_type) {
	//	case PNG_COLOR_TYPE_PALETTE:
	//		png_set_palette_to_rgb(png_ptr);
	//		//Don't forget to update the channel info (thanks Tom!)
	//		//It's used later to know how big a buffer we need for the image
	//		channels = 3;
	//		break;
	//	case PNG_COLOR_TYPE_GRAY:
	//		if (bits_per_channel < 8)
	//			png_set_expand_gray_1_2_4_to_8(png_ptr);
	//		//And the bitdepth info
	//		bits_per_channel = 8;
	//		break;
	//}

	///*if the image has a transperancy set.. convert it to a full Alpha channel..*/
	//if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)) {
	//	png_set_tRNS_to_alpha(png_ptr);
	//	channels += 1;
	//}

	//imageData.channels = channels;

	//number_of_passes = png_set_interlace_handling(png_ptr);
	//png_read_update_info(png_ptr, info_ptr);

	///* read file */
	//if (setjmp(png_jmpbuf(png_ptr)))
	//	//abort_("[read_png_file] Error during read_image");
	//	return ImageLoaderStatus::UNDEFINED;

	//auto stride = imageData.width * channels * bits_per_channel / 8;
	//auto bufferSize = sizeof(png_bytep) * imageData.height * stride;
	//imageData.data = new unsigned char[ bufferSize ];

	//auto rowbytes = png_get_rowbytes(png_ptr, info_ptr);
	//
	//auto rowPtrs = new png_bytep[imageData.height];
	//for (size_t i = 0; i < imageData.height; i++) {
	//	rowPtrs[i] = (png_bytep)imageData.data[i * stride];
	//}

	//png_read_image(png_ptr, rowPtrs);

	//delete[]rowPtrs;

	//fclose(fp);
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
