#pragma once

#include"glm/glm.hpp"

class Hexagon
{
public:
	Hexagon();
	void Generate(const glm::vec2& v, float innerRadius, float outerRadius);
	void Render() const;
	
	void SetColor(const glm::vec3& color) { this->color = color; this->highlight = color; }
	void NoHighlight() { this->highlight = this->color; }
	void SetHighlight(const glm::vec3& color) { this->highlight = color; }

private:
	static constexpr unsigned int num_points = (6 * 3 * 2) + 4 * 3;
	glm::vec3 points[num_points];
	glm::vec3 normals[num_points];

	glm::vec3 color;
	glm::vec3 highlight;
};
