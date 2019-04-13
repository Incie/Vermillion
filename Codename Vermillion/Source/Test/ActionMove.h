#pragma once

#include"Level.h"

class ActionMove
{
public:
	ActionMove(Level& level, glm::ivec3 center, int moveMax);
	~ActionMove();

	void Click(const glm::ivec3& target);
	void Undo();
	void Reset();
	void Perform(Actor& actor);


	void Render();

private:
	void Highlight();

	std::vector<glm::ivec3> plannedRoute;

	glm::ivec3 start;
	glm::ivec3 currentPosition;
	const int moveMax;
	int movesLeft;

	Level &level;
};

