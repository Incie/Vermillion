#include"pch.h"
#include"../entity/Entity.h"
#include"EnemyAI.h"
#include"EnemyRound.h"
#include"EnemyAction.h"
#include"../level/Level.h"
#include"GL/glew.h"
#include"../../Framework/services.h"

EnemyAI::EnemyAI()
	: actor(nullptr), enemyRound(nullptr)
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
		enemyRound->state = EnemyRound::RoundState::Stopped;
	}
}

void EnemyAI::SetActor(Actor* actor)
{
	this->actor = actor;

	if( enemyRound != nullptr ){
		enemyRound->Reset();
		enemyRound->state = EnemyRound::RoundState::Stopped;
	}
}

bool EnemyAI::Finished()
{
	return enemyRound->state == EnemyRound::RoundState::Finished;
}

void EnemyAI::Step(Level& level)
{
	if (enemyRound->state == EnemyRound::RoundState::Stopped) {
		CalculateStep(level);
	}
	else if (enemyRound->state == EnemyRound::RoundState::Calculated) {
		PerformStep(level);
	}
	else if (enemyRound->state == EnemyRound::RoundState::Finished) {
	}
}

void EnemyAI::CalculateStep(Level& level)
{
	if (enemyRound->HasNextAction()) {
		enemyRound->NextAction();

		auto action = enemyRound->GetAction();
		action->Calculate(level , *actor);
		enemyRound->state = EnemyRound::RoundState::Calculated;
	}
}

void EnemyAI::PerformStep(Level& level)
{
	auto action = enemyRound->GetAction();
	action->Perform(level, *actor);

	if (enemyRound->HasNextAction()) {
		enemyRound->state = EnemyRound::RoundState::Stopped;
		CalculateStep(level);
	}
	else 
		enemyRound->state = EnemyRound::RoundState::Finished;
}

void EnemyAI::Render()
{
	if(enemyRound != nullptr)
		enemyRound->Render();
}