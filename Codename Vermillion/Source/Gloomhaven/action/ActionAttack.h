#pragma once

#include"Action.h"

class Level;

class ActionAttack : public Action
{
public:
	ActionAttack(Level& level, Actor& actor, int range, int attackDamage, int attacks);
	~ActionAttack();

	virtual void Click(const glm::ivec3& target) override;
	virtual void Undo() override;
	virtual void Reset() override;
	virtual bool Perform(Actor& actor);

	virtual void Render() override;

	void Highlight();
	void NoHighlight();

	bool IsMarked(int entityId);
private:

	std::vector<glm::ivec3> marks;
	int range;
	int attacks;
	int baseDamage;
};