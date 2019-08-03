#pragma once

#include"glm/glm.hpp"
#include<utility>
#include<tuple>

struct collisionpacket {
	collisionpacket(){ collision = false; fraction = 1.0f; normal = glm::vec2{1,0}; };
	collisionpacket(bool collision, float fraction, const glm::vec2 &normal):collision(collision), fraction(fraction), normal(normal){}

	static collisionpacket NoCollide() {
		static collisionpacket nocollision{false, 1.0f, glm::vec2{1,0}};
		return nocollision; 
	}

	bool collision;
	float fraction;
	glm::vec2 normal;
};

class Plane 
{
public:
	Plane(){}
	Plane(const glm::vec2& p0, const glm::vec2& p1);

	void Set(const glm::vec2& p0, const glm::vec2& p1);

	collisionpacket Collide(const glm::vec2& position, const glm::vec2& velocity, float radius);

	collisionpacket CollidePoint(const glm::vec2& position, const glm::vec2& velocity, float radius, const glm::vec2& point);

	glm::vec2 Normal() { return planeNormal; }
private:
	glm::vec2 point0;
	glm::vec2 point1;
	glm::vec2 lineSegment;
	glm::vec2 lineSegmentNormalized;
	float lineLength;
	glm::vec2 planeNormal;
	float planeDistance;
};