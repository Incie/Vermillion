#pragma once

#include"glm/glm.hpp"

class Texture;

class Render
{
public:
	static float Quad(const Texture& texture, float height);
	static void Quad(float x, float y, float w, float h, const glm::vec3& color);
};