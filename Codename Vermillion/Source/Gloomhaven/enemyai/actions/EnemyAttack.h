#pragma once


#include"EnemyAction.h"
#include"../../entity/StatusEffect.h"
#include<vector>

//Attack eligible targets
//post-hit effects
class EnemyAttack : public EnemyAction {
public:
	EnemyAttack(int attack, int range, int targets = 1, std::vector<StatusEffect> statusEffects = std::vector<StatusEffect>{});

	bool CanPerform(const Actor& actor) override;
	void Calculate(Level& level, const Actor& actor) override;
	void Perform(Director& director, Level& level, Actor& actor) override;
	void Render() override;

	virtual void Modify(const Actor& actor) override;
private:
	int originalRange;
	int range;

	int originalAttack;
	int attack;

	int originalTargets;
	int attackTargets;

	std::vector<StatusEffect> statusEffects;
};