#pragma once

#include"glm/glm.hpp"
#include<utility>

class Plane 
{
public:
	Plane(){}
	Plane(const glm::vec2& p0, const glm::vec2& p1);

	std::pair<bool, float> Collide(const glm::vec2& position, const glm::vec2& velocity, float radius);

private:
	glm::vec2 point0;
	glm::vec2 point1;
	glm::vec2 lineSegment;
	glm::vec2 lineSegmentNormalized;
	float lineLength;
	glm::vec2 planeNormal;
	float planeDistance;
};