#include"pch.h"
#include"EnemyAction.h"


EnemyAction::EnemyAction()
	: state(0)
{
}

EnemyAction::~EnemyAction()
{
	targets.clear();
}
