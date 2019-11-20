#include"pch/pch.h"
#include"../entity/Entity.h"
#include"EnemyAI.h"
#include"EnemyRound.h"
#include"actions/EnemyAction.h"
#include"../level/Level.h"
#include"GL/glew.h"
#include"services.h"

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
	auto finished = enemyRound->state == EnemyRound::RoundState::Finished;

	if(finished)
		actor->Moved();

	return finished;
}

void EnemyAI::Step(Director& director, Level& level)
{
	if (enemyRound->state == EnemyRound::RoundState::Stopped) {
		CalculateStep(level);
	}
	else if (enemyRound->state == EnemyRound::RoundState::Calculated) {
		PerformStep(director, level);
	}
	else if (enemyRound->state == EnemyRound::RoundState::Finished) {
	}
}

void EnemyAI::CalculateStep(Level& level)
{
	if (enemyRound->HasNextAction()) {
		enemyRound->NextAction();

		auto action = enemyRound->GetAction();

		if(!action->CanPerform(*actor)) {
			auto combatMessage = fmt::format("{0} could not perform action {1}", actor->Name(), action->ActionDescription() );
			level.combatLog.push_back(combatMessage);
			NextAction(level);
		}

		action->Calculate(level , *actor);
		enemyRound->state = EnemyRound::RoundState::Calculated;
	}
}

void EnemyAI::PerformStep(Director& director, Level& level)
{
	auto action = enemyRound->GetAction();
	action->Perform(director, level, *actor);
	
	NextAction(level);
}

void EnemyAI::NextAction(Level& level)
{
	if(enemyRound->HasNextAction()) {
		enemyRound->state = EnemyRound::RoundState::Stopped;
		CalculateStep(level);
		return;
	}
	
	enemyRound->state = EnemyRound::RoundState::Finished;
}

void EnemyAI::Render()
{
	if(enemyRound != nullptr)
		enemyRound->Render();
}