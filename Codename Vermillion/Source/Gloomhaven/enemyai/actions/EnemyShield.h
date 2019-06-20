#pragma once

#include"EnemyAction.h"
#include"../../entity/StatusEffect.h"

//Strengthen (end of next turn)
//Shield (end of round)
//Retaliate (end of round)
class EnemySelfCast : public EnemyAction {
public:
	EnemySelfCast(StatusEffect effect);

	virtual bool CanPerform(const Actor& actor) override;
	virtual void Calculate(Level& level, const Actor& actor) override;
	virtual void Perform(Director& director, Level& level, Actor& actor) override;
	void Render() override;

private:
	StatusEffect effect;
};

class EnemyHealSelf : public EnemyAction {
public:
	EnemyHealSelf(int healAmount);

	virtual bool CanPerform(const Actor& actor) override;
	virtual void Calculate(Level& level, const Actor& actor) override;
	virtual void Perform(Director& director, Level& level, Actor& actor) override;
	void Render() override;

private:
	int heal;
	glm::vec3 actorWorldPosition;
};

class EnemyShieldSelf : public EnemyAction {
public:
	EnemyShieldSelf(int shield);

	virtual bool CanPerform(const Actor& actor) override;
	virtual void Calculate(Level& level, const Actor& actor) override;
	virtual void Perform(Director& director, Level& level, Actor& actor) override;
	void Render() override;

private:
	int shield;
};

class EnemyRetaliate : public EnemyAction {
public:
	EnemyRetaliate(int retaliate);

	virtual bool CanPerform(const Actor& actor) override;
	virtual void Calculate(Level& level, const Actor& actor) override;
	virtual void Perform(Director& director, Level& level, Actor& actor) override;
	void Render() override;

private:
	int retaliate;
};