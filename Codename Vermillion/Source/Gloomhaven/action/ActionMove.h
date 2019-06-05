#pragma once

#include"Action.h"

class Level;

class ActionMove : public Action
{
public:
	ActionMove(Level& level, Actor& actor, int moveMax, bool flying=false);
	virtual ~ActionMove();

	virtual void Click(const glm::ivec3& target) override;
	virtual void Undo() override;
	virtual void Reset() override;
	virtual bool Perform(Actor& actor) override;

	virtual void Render() override;

	virtual void Highlight() override;
protected:

	std::vector<glm::ivec3> plannedRoute;

	glm::ivec3 currentPosition;
	const int moveMax;
	int movesLeft;
	bool flying;
};

class ActionTrample : public ActionMove {
public:
	ActionTrample(Level& level, Actor& actor, int moveMax, bool flying, int attack);
	virtual ~ActionTrample();

	virtual bool Perform(Actor& actor) override;

protected:
	int attack;
};

