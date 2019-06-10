#pragma once

#include"glm/glm.hpp"
#include<string>
#include<vector>

class Level;
class Actor;

class EnemyAction {
public:
	EnemyAction(Level& level);
	virtual ~EnemyAction();

	virtual bool CanPerform(const Actor& actor) { return true; }
	virtual void Calculate(const Actor& actor) = 0;
	virtual std::vector<glm::ivec3> Targets();
	virtual void Perform(Actor& actor) {};

	virtual void Render() {}
	virtual void Reset() { state = 0; targets.clear(); }
	int state;

	const std::string ActionDescription() { return actionDescription; }
protected:
	std::string actionDescription;
	Level& level;
	glm::vec2 startPoint;
	std::vector<glm::ivec3> targets;
};

//Move towards focus
//landing effects?
class EnemyMove : public EnemyAction {
public:
	EnemyMove(Level& level, int move);

	bool CanPerform(const Actor& actor) override;
	void Calculate(const Actor& actor) override;
	void Perform(Actor& actor) override;
	void Render();
protected:

	int move;
};

//Attack eligible targets
//post-hit effects
class EnemyAttack : public EnemyAction {
public:
	EnemyAttack(Level& level, int attack, int range);

	bool CanPerform(const Actor& actor) override;
	void Calculate(const Actor& actor) override;
	void Perform(Actor& actor) override;
	void Render() override;

private:
	int range;
	int attack;
};

//Strengthen (end of next turn)
//Shield (end of round)
//Retaliate (end of round)
class EnemySelfCast {};