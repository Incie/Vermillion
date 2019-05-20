#pragma once


#include"Action.h"

class ActionShieldSelf : public Action {
public:
	ActionShieldSelf(Level& level, Actor& actor, int shield);
	virtual ~ActionShieldSelf();

	virtual void Click(const glm::ivec3& target);
	virtual void Undo();
	virtual void Reset();
	virtual bool Perform(Actor& actor) ;
	virtual void Render();
private:
	int shield;
};