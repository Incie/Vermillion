#pragma once

#include"math.h"
#include"glm/glm.hpp"
#include<array>

class Brick {
public:
	Brick(const glm::vec2& center, const glm::vec2& size);
	~Brick() = default;

	std::tuple<bool, float, glm::vec2> Collide(const glm::vec2& position, const glm::vec2& velocity, float radius);

	void Update(float delta);
	void Render() const;

	bool Active() { return active; }
	void Deactivate() { active = false;}
private:
	Brick() = default;

	bool active;
	std::array<glm::vec2, 4> points;
	std::array<Plane, 4> planes;
};