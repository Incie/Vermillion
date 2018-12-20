#pragma once

#include"glm/glm.hpp"

class Hexagon
{
public:
	Hexagon(const glm::vec2 &v, double innerRadius, double outerRadius);
	void Render();

	void SetColor(const glm::vec3& color) { this->color = color; }

	double DistanceFromCenterTo(const glm::vec2& point) const;

	glm::vec3 center;
	glm::ivec3 cubeCoordinate;
private:
	static constexpr unsigned int num_points = (6 * 3 * 2) + 4 * 3;
	glm::vec3 points[num_points];
	glm::vec3 normals[num_points];

	glm::vec3 color;
};
