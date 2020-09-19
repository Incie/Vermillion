#pragma once

#include"EnemyAction.h"

//Move towards focus
//landing effects?
class EnemyMove : public EnemyAction {
public:
	EnemyMove(int move);

	virtual bool CanPerform(const Actor& actor) override;
	virtual void Calculate(Level& level, const Actor& actor) override;
	virtual void Perform(Director& director, Level& level, Actor& actor) override;
	virtual void Render();

	virtual void Modify(const Actor& actor) override;
protected:

	int originalMove;
	int move;
};