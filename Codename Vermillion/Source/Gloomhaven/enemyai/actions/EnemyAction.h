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
	virtual void Reset() { calculated = false;  state = 0; targets.clear(); }
	int state;

	virtual void Modify(const Actor& actor) {}

	const std::string ActionDescription() { return actionDescription; }
protected:
	bool calculated;
	glm::vec3 actorWorldPosition;

	std::string actionDescription;
	glm::vec2 startPoint;
	std::vector<std::pair<glm::ivec3, glm::vec3>> targets;
};