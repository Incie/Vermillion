#include"pch.h"
#include"EnemyAI.h"
#include"EnemyRound.h"
#include"EnemyAction.h"
#include"../level/Level.h"
#include"GL/glew.h"
#include"../../Framework/services.h"

EnemyAI::EnemyAI(Level& level)
	: level(level)
{
}

EnemyAI::~EnemyAI()
{
}

void EnemyAI::SetRoundActions(EnemyRound* enemyRound)
{
	this->enemyRound = enemyRound;
}

void EnemyAI::SetActor(Actor* actor)
{
	this->actor = actor;
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
		//next actor
		enemyRound->Reset();
		enemyRound->state = EnemyRound::State::Stopped;
	}
}

void EnemyAI::CalculateStep()
{
	if (enemyRound->HasNextAction()) {
		enemyRound->NextAction();

		auto action = enemyRound->GetAction();
		action->Calculate(*level.ActorById(2));
		enemyRound->state = EnemyRound::State::Calculated;
	}
}

void EnemyAI::PerformStep()
{
	auto action = enemyRound->GetAction();
	auto& actor = *level.ActorById(2);
	action->Perform(actor);

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