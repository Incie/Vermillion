#include"pch.h"
#include"../entity/Entity.h"
#include"EnemyAI.h"
#include"EnemyRound.h"
#include"EnemyAction.h"
#include"../level/Level.h"
#include"GL/glew.h"
#include"../../Framework/services.h"

EnemyAI::EnemyAI(Level& level)
	: level(level), actor(nullptr), enemyRound(nullptr)
{
}

EnemyAI::~EnemyAI()
{
}

void EnemyAI::SetRoundActions(EnemyRound* enemyRound)
{
	this->enemyRound = enemyRound;

	if (enemyRound != nullptr) {
		enemyRound->Reset();
		enemyRound->state = EnemyRound::State::Stopped;
	}
}

void EnemyAI::SetActor(Actor* actor)
{
	this->actor = actor;

	if( enemyRound != nullptr ){
		enemyRound->Reset();
		enemyRound->state = EnemyRound::State::Stopped;
	}
}

bool EnemyAI::Finished()
{
	return enemyRound->state == EnemyRound::State::Finished;
}

void EnemyAI::Step()
{
	if (enemyRound->state == EnemyRound::State::Stopped) {
		CalculateStep();
	}
	else if (enemyRound->state == EnemyRound::State::Calculated) {
		PerformStep();
	}
	else if (enemyRound->state == EnemyRound::State::Finished) {
	}
}

void EnemyAI::CalculateStep()
{
	if (enemyRound->HasNextAction()) {
		enemyRound->NextAction();

		auto action = enemyRound->GetAction();
		action->Calculate(*actor);
		enemyRound->state = EnemyRound::State::Calculated;
	}
}

void EnemyAI::PerformStep()
{
	auto action = enemyRound->GetAction();
	action->Perform(*actor);

	if (enemyRound->HasNextAction()) {
		enemyRound->state = EnemyRound::State::Stopped;
		CalculateStep();
	}
	else 
		enemyRound->state = EnemyRound::State::Finished;
}

void EnemyAI::Render()
{
}