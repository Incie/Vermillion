#pragma once

#include"glm/glm.hpp"
class Actor;
class Level;

class Action {
public:
	Action(Level& level, Actor& actor) :level(level), actor(actor) { actionDescription = "Action"; }
	virtual ~Action() {};

	virtual void Click(const glm::ivec3& target) = 0;
	virtual void Undo() = 0;
	virtual void Reset() = 0;
	virtual bool Perform(Actor& actor) = 0;
	virtual void Render() = 0;

	virtual void Highlight() {}

	const std::string& Description() { return actionDescription; }
protected:
	Actor& actor;
	Level& level;

	std::string actionDescription;
};