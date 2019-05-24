#pragma once

#include"glm/glm.hpp"

class Triangle 
{
public:
	Triangle();
	Triangle(const Triangle& triangle);
	Triangle(float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2);
	Triangle(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2);
	~Triangle();

	void Transform(const glm::mat4& transform);

	glm::vec4 vertices[3];
};