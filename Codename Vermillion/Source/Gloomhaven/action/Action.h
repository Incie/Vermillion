#pragma once

#include"glm/glm.hpp"
#include<string>

class Director;
class Actor;
class Level;

class Action {
public:
	Action(Level& level, Actor& actor) :level(level), actor(actor) { actionDescription = "Action"; }
	virtual ~Action() {};

	virtual void Click(Director& director, const glm::ivec3& target) = 0;
	virtual void Undo() = 0;
	virtual void Reset() = 0;
	virtual bool Perform(Director& director, Actor& actor) = 0;
	virtual void Render() = 0;

	virtual void Highlight() {}

	const std::string& Description() { return actionDescription; }
protected:
	Actor& actor;
	Level& level;

	std::string actionDescription;
};


class ActionElement : public Action {
public:
	ActionElement(Level& level, Actor& actor);

	virtual void Click(Director& director, const glm::ivec3& target) override;
	virtual void Undo() override;
	virtual void Reset() override;
	virtual bool Perform(Director& director, Actor& actor) override;
	virtual void Render() override;
};