#pragma once

#include"pch.h"

namespace Colors {
	const glm::vec3 White { 1.0f };	
	const glm::vec3 Black { 0.0f };
	const glm::vec3 Red { 1.0f, 0.0f, 0.0f };
	const glm::vec3 Green { 0.0f, 1.0f, 0.0f };
	const glm::vec3 Blue { 0.0f, 0.0f, 1.0f };
	const glm::vec3 Gray { 0.5f, 0.5f, 0.5f };
	const glm::vec3 Yellow { 1.0f, 1.0f, 0.0f };


	glm::vec4 ColorFromRGB(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
		return glm::vec4(
			static_cast<float>(r) / 255.0f,
			static_cast<float>(g) / 255.0f,
			static_cast<float>(b) / 255.0f,
			static_cast<float>(a) / 255.0f);
	}

	glm::vec4 ColorFromRGB(unsigned char r, unsigned char g, unsigned char b) {
		return glm::vec4( 
			static_cast<float>(r) / 255.0f, 
			static_cast<float>(g) / 255.0f, 
			static_cast<float>(b) / 255.0f, 
			1.0f);
	}

	glm::vec4 ColorFromRGB(unsigned char color) {
		return glm::vec4(
			static_cast<float>(color) / 255.0f,
			static_cast<float>(color) / 255.0f,
			static_cast<float>(color) / 255.0f,
			1.0f);
	}
};

namespace Paths {
	const FilePath Data = FilePath(1, "../Data/");
	const FilePath Fonts = FilePath(Data, "fonts/");
	const FilePath Textures = FilePath(Data, "textures/");
	const FilePath Shaders = FilePath(Data, "shaders/");
	const FilePath Levels = FilePath(Data, "levels/");
	
	const FilePath Sounds = FilePath(Data, "sounds/");
	const FilePath Music = FilePath(Data, "music/");
};