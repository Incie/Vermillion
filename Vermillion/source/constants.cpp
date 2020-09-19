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
};

namespace Paths {
	const FilePath Data = FilePath(1, "../Data/");
	const FilePath Fonts = FilePath(Data, "fonts/");
	const FilePath Textures = FilePath(Data, "textures/");
	const FilePath Shaders = FilePath(Data, "shaders/");
	const FilePath Levels = FilePath(Data, "levels/");
};