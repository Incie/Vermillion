#include"pch.h"
#include "Director.h"
#include"../services.h"

#include"action/Action.h"
#include"action/ActionAttack.h"
#include"action/ActionMove.h"
#include"level/Level.h"

#include"enemyai/EnemyAI.h"
#include"enemyai/EnemyAction.h"
#include"enemyai/EnemyRound.h"

#include<Windows.h>
#include"GL/glew.h"


Director::Director(Level& level)
	: level(level), action(nullptr), enemyAi(level)
{

	enemyRound = new EnemyRound();
	enemyRound->AddAction(new EnemyMove(level, 2));
	enemyRound->AddAction(new EnemyAttack(level, 2, 1));

	enemyAi.SetActor(level.ActorById(2));
	enemyAi.SetRoundActions(enemyRound);
}

Director::~Director()
{
	if( enemyRound )
		delete enemyRound;

	if (action)
		delete action;
}

void Director::Update(const InputService& input)
{
	if (initiativeTracker.EnemyTurn())
		EnemyTurn(input);
	else 
		PlayerTurn(input);

	if (input.KeyOnce('K') && initiativeTracker.RoundFinished()) {
		initiativeTracker.CalculateRoundOrder(level);
		initiativeTracker.NextActor();
	}
}

void Director::Render()
{
	if (action)
		action->Render();

	if( enemyRound )
		enemyRound->Render();
}

void Director::RenderUI(const TextService& text)
{
	if (initiativeTracker.RoundFinished()) {
		text.Print(500, 25, "ROUND FINISHED", 25, Colorf(1));
	}

	glPushMatrix();
		glTranslatef(5.0f, 300.0f, 0.0f);
		enemyRound->RenderRoundCard(text);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(512.0f, 0, 0);
		initiativeTracker.Render(text);
	glPopMatrix();

	if (action != nullptr) {
		text.Print(200, 25, action->Description(), 20, Colorf(1, 1, 1));
	}
}

void Director::PlayerTurn(const InputService& input)
{
	if (action == nullptr && input.KeyOnce(VK_F1)) {
		action = new ActionMove(level, *level.GetPlayer(), 6);
	}

	if (action == nullptr && input.KeyOnce(VK_F2)) {
		action = new ActionAttack(level, *level.GetPlayer(), 1, 4, 1);
	}

	if (input.KeyOnce(VK_SPACE)) {
		if (action == nullptr) {
			auto a = initiativeTracker.NextActor();

			if (initiativeTracker.EnemyTurn()) {
				enemyAi.SetActor(a);
				enemyAi.SetRoundActions(enemyRound);
			}
		}
	}

	if (action != nullptr && input.KeyOnce(VK_RETURN)) {
		if (action->Perform(*level.GetPlayer())) {
			delete action;
			action = nullptr;
		}
	}

	if (action != nullptr && input.KeyOnce(VK_LBUTTON)) {
		if (level.HasHoverTarget())
			action->Click(level.GetHoverTarget().Location());
	}

	if (action != nullptr && input.KeyOnce(VK_BACK)) {
		action->Undo();
	}

	if (action != nullptr && input.KeyOnce(VK_ESCAPE)) {
		action->Reset();
		delete action;
		action = nullptr;
		level.ClearHighlights();
	}
}

void Director::EnemyTurn(const InputService& input)
{
	if (input.KeyOnce('N')) {
		enemyAi.Step();

		if (enemyAi.Finished()) {
			auto a = initiativeTracker.NextActor();

			if (initiativeTracker.EnemyTurn()) {
				enemyAi.SetActor(a);
				enemyAi.SetRoundActions(enemyRound);
			}
		}
	}
}
