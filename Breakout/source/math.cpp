#include "math.h"
#include"render.h"

Plane::Plane(const glm::vec2& p0, const glm::vec2& p1)
{
	auto lineSegment = p1 - p0;
	auto lineNormal = glm::normalize( glm::vec2{lineSegment.y, -lineSegment.x} );

	auto distance = glm::dot(p0 * -1.0f, lineNormal);
	
	this->point0 = p0;
	this->point1 = p1;
	this->lineSegment = lineSegment;
	this->lineSegmentNormalized = lineNormal;
	this->lineLength = glm::length(lineSegment);
	this->planeNormal = lineNormal;
	this->planeDistance = distance;	
}

std::pair<bool, float> Plane::Collide(const glm::vec2& position, const glm::vec2& velocity, float radius)
{
	auto d0 = glm::dot(position, planeNormal) + planeDistance;
	auto d1 = glm::dot(position+velocity, planeNormal) + planeDistance;

	bool collision = d0>radius && d1<radius;

	float collisionPoint = 1.0f;
	collisionPoint = (d0-radius)/(d0-d1);

	auto planeCollisionPoint = position + velocity*collisionPoint - planeNormal*radius;

	auto distanceToP0 = glm::distance(planeCollisionPoint, point0);
	auto distanceToP1 = glm::distance(planeCollisionPoint, point1);
	if( distanceToP0 > lineLength || distanceToP1 > lineLength ){
		collision = false;
		collisionPoint = 1.0f;

		auto p0Col = CollidePoint(position, velocity, radius, point0);

		if( p0Col.first ) {
			if( p0Col.second < collisionPoint ) {
				collision = true;
				collisionPoint = p0Col.second;
			}
		}

		auto p1Col = CollidePoint(position, velocity, radius, point1);

		if( p1Col.first ) {
			if( p1Col.second < collisionPoint ) {
				collision = true;
				collisionPoint = p1Col.second;
			}
		}

		if( collision )
			return std::pair(true, collisionPoint);
	}

	if( collisionPoint < 0.0f || collisionPoint > 1.0f )
		return std::pair(false, 1.0f);

	return std::pair(collision, collisionPoint);
}

std::pair<bool, float> Plane::CollidePoint(const glm::vec2& position, const glm::vec2& velocity, float radius, const glm::vec2& point)
{
		auto velocityNormalized = glm::normalize(velocity);

		auto dot0 = glm::dot(velocityNormalized, point - position) / glm::length(velocity); 
		auto p0Projected = position + velocity * dot0;

		auto p0ProjectedDistance = glm::length(point - p0Projected);
		auto colPoint0SquaredDistance = radius*radius - p0ProjectedDistance*p0ProjectedDistance;


		if( colPoint0SquaredDistance < 0.0f )
			return std::pair(false, 1.0f);


		auto colPoint0Distance = glm::sqrt(glm::abs(colPoint0SquaredDistance));


		Render::Circle(p0Projected, 5, {1.0f, 0, 0});
		Render::Line(point, p0Projected, {0,1,0});

		if( colPoint0Distance > radius )
			return std::pair(false, 1.0f);
		
		auto p0Col = p0Projected - velocityNormalized * colPoint0Distance;

		Render::Circle(p0Col, 5, {1.0f, 0, 0});
		Render::Line(point, p0Col, {0,1,0});
		
		auto velMagnitude = glm::length(velocity);
		auto colMagnitude = glm::length(p0Col - position);

		if( colMagnitude < velMagnitude )
			return std::pair(true, colMagnitude / velMagnitude);
		
		return std::pair(false, 1.0f);
}
