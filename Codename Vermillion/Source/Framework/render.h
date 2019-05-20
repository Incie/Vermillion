#pragma once

#include"glm/glm.hpp"

class Texture;

class Render
{
public:
	static float Quad(const glm::vec2& position, Texture& texture, float height);
	static float Quad(const Texture& texture, float height);
	static void Quad(const glm::vec2& position, const glm::vec2& size, const glm::vec3& color);
	static void Quad(float x, float y, float w, float h, const glm::vec3& color);


	static const glm::vec3 WHITE;
	static const glm::vec3 BLACK;
};