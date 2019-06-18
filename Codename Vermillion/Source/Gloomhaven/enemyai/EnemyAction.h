#pragma once

#include"glm/glm.hpp"
#include<string>
#include<vector>

class Director;
class Level;
class Actor;

class EnemyAction {
public:
	EnemyAction();
	virtual ~EnemyAction();

	virtual bool CanPerform(const Actor& actor) { return true; }
	virtual void Calculate(Level& level, const Actor& actor) = 0;
	virtual void Perform(Director& director, Level& level, Actor& actor) {};

	virtual std::vector<std::pair<glm::ivec3, glm::vec3>> Targets();

	virtual void Render() {}
	virtual void Reset() { state = 0; targets.clear(); }
	int state;

	const std::string ActionDescription() { return actionDescription; }
protected:
	std::string actionDescription;
	glm::vec2 startPoint;
	std::vector<std::pair<glm::ivec3, glm::vec3>> targets;
};

//Move towards focus
//landing effects?
class EnemyMove : public EnemyAction {
public:
	EnemyMove(int move);

	bool CanPerform(const Actor& actor) override;
	void Calculate(Level& level, const Actor& actor) override;
	void Perform(Director& director, Level& level, Actor& actor) override;
	void Render();
protected:

	int move;
};

//Attack eligible targets
//post-hit effects
class EnemyAttack : public EnemyAction {
public:
	EnemyAttack(int attack, int range);

	bool CanPerform(const Actor& actor) override;
	void Calculate(Level& level, const Actor& actor) override;
	void Perform(Director& director, Level& level, Actor& actor) override;
	void Render() override;

private:
	int range;
	int attack;
};

//Strengthen (end of next turn)
//Shield (end of round)
//Retaliate (end of round)
class EnemySelfCast {};

class EnemyHealSelf : public EnemyAction {
public:
	EnemyHealSelf(int healAmount);

	virtual bool CanPerform(const Actor& actor) override;
	virtual void Calculate(Level& level, const Actor& actor) override;
	virtual void Perform(Director& director, Level& level, Actor& actor) override;
	void Render() override;

private:
	int heal;
	bool calculated;
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
	bool calculated;
	glm::vec3 actorWorldPosition;
};

class EnemyRetaliate : public EnemyAction {
public:
	EnemyRetaliate(int retliate);

	virtual bool CanPerform(const Actor& actor) override;
	virtual void Calculate(Level& level, const Actor& actor) override;
	virtual void Perform(Director& director, Level& level, Actor& actor) override;
	void Render() override;

private:
	int retaliate;
	bool calculated;
	glm::vec3 actorWorldPosition;
};