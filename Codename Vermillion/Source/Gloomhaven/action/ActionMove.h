#pragma once

#include"Action.h"

class Level;

class ActionMove : public Action
{
public:
	ActionMove(Level& level, Actor& actor, int moveMax);
	virtual ~ActionMove();

	virtual void Click(const glm::ivec3& target) override;
	virtual void Undo() override;
	virtual void Reset() override;
	virtual bool Perform(Actor& actor) override;

	void Render() override;

	void Highlight();
private:

	std::vector<glm::ivec3> plannedRoute;

	glm::ivec3 currentPosition;
	const int moveMax;
	int movesLeft;
};

