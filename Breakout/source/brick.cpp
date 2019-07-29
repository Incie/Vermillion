#include "brick.h"
#include"render.h"

Brick::Brick(const glm::vec2& center, const glm::vec2& size)
{
	active = true;

	auto halfSize = size * 0.5f;
	points[0] = center - halfSize;
	points[1] = {center.x + halfSize.x, center.y - halfSize.y };
	points[2] = center + halfSize;
	points[3] = {center.x - halfSize.x, center.y + halfSize.y };

	planes[0] = Plane(points[0], points[1]);
	planes[1] = Plane(points[1], points[2]);
	planes[2] = Plane(points[2], points[3]);
	planes[3] = Plane(points[3], points[0]);
}

std::tuple<bool, float, glm::vec2> Brick::Collide(const glm::vec2& position, const glm::vec2& velocity, float radius)
{
	bool collision = false;
	float fraction = 1.0f;
	glm::vec2 normal{0,0};

	for( auto& plane : planes ){
		auto [collided, planeFraction] = plane.Collide(position, velocity, radius);

		if( collided && planeFraction < fraction ){
				collision = true;
				fraction = planeFraction;
				normal = plane.Normal();
		}
	}

	return std::tuple(collision, fraction, normal);
}

void Brick::Update(float delta)
{
}

void Brick::Render() const 
{
	if( !active )
		return;

	Render::Rectangle(points[0], points[1], points[2], points[3], glm::vec3{1,1,1});
}
