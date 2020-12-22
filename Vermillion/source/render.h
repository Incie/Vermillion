export module render;

#pragma once

#include"glm/glm.hpp"

class Texture;

class Render
{
public:
	static void Quad(const glm::vec2& position, const glm::vec2& size, const Texture& texture, const glm::vec3& color, bool center = false);
	static float Quad(const glm::vec2& position, Texture& texture, float height);
	static float Quad(const Texture& texture, float height);

	static void Quad(const glm::vec2& position, const glm::vec2& size, const glm::vec3& color);
	static void Quad(float x, float y, float w, float h, const glm::vec3& color);

	static void Quads(const glm::vec2* coords, const glm::vec3* colors, int vertexCount);

	static void Rectangle(const glm::vec2& p0, const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3, const glm::vec3& color);
	static void Rectangle(const glm::vec2& position, const glm::vec2& size, const glm::vec3& color);
	static void Circle(const glm::vec2& position, float radius, const glm::vec3& color);
	static void Line(const glm::vec2& point0, const glm::vec2& point1, const glm::vec3& color);

	static void LineGrid(const glm::vec2& from, const glm::vec2& to, const glm::vec3& color, float step);

	static void OriginCross(float size);

	static void PushMatrix();
	static void PopMatrix();
	static void Translate3D(const glm::vec3& p);
	static void Translate2D(const glm::vec3& p);
	static void Translate2D(const glm::vec2& p);
	static void Scale(glm::vec3& s);
	static void Rotate2D(float angles);

	static void WireFrame();
	static void Fill();

	static void EnableLight();
	static void NoLight();
	static void SetLightData();
};