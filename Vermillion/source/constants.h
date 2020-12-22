#pragma once

namespace Colors {
	#include"glm/vec3.hpp"	

	extern const glm::vec3 White;	
	extern const glm::vec3 Black;
	extern const glm::vec3 Red;
	extern const glm::vec3 Yellow;
	extern const glm::vec3 Gray;
	extern const glm::vec3 Green;
	extern const glm::vec3 Blue;


	glm::vec4 ColorFromRGB(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	glm::vec4 ColorFromRGB(unsigned char r, unsigned char g, unsigned char b);
	glm::vec4 ColorFromRGB(unsigned char color);
};

namespace Paths {
	#include"file.h"

	extern const FilePath Data;
	extern const FilePath Fonts;
	extern const FilePath Textures;
	extern const FilePath Shaders;
	extern const FilePath Levels;
	extern const FilePath Sounds;
	extern const FilePath Music;
};