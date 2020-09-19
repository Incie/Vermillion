#pragma once


#include"math.h"
#include"glm/glm.hpp"
#include<array>

class Paddle {
public:
	Paddle(const glm::vec2& center, const glm::vec2& size);
	~Paddle() = default;

	std::tuple<bool, float, glm::vec2> Collide(const glm::vec2& position, const glm::vec2& velocity, float radius);

	glm::vec2 Position() { return position; }
	void SetCenterPosition(const glm::vec2& center);

	void Update(float delta);
	void Render() const;

private:
	Paddle() = default;

	glm::vec2 position;
	glm::vec2 paddleSize;
	std::array<glm::vec2, 4> points;
	std::array<Plane, 4> planes;
};