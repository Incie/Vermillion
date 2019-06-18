#pragma once

#include"Action.h"
#include<vector>
#include"..//entity/Entity.h"

class Level;
class Director;

class ActionAttack : public Action
{
public:
	ActionAttack(Level& level, Actor& actor, int range, int attackDamage, int attacks, std::vector<StatusEffect> statusEffects = std::vector<StatusEffect>() );
	~ActionAttack();

	virtual void Click(Director& director, const glm::ivec3& target) override;
	virtual void Undo() override;
	virtual void Reset() override;
	virtual bool Perform(Director& director, Actor& actor);

	virtual void Render() override;

	void Highlight();
	void NoHighlight();

	bool IsMarked(int entityId);
private:


	std::vector<glm::ivec3> marks;
	
	std::vector<StatusEffect> statusEffects;
	int range;
	int attacks;
	int baseDamage;
};