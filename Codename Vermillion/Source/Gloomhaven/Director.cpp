#include"pch.h"
#include "Director.h"
#include"../services.h"

#include"action/Action.h"
#include"action/ActionAttack.h"
#include"action/ActionMove.h"
#include"action/ActionShieldSelf.h"
#include"level/Level.h"

#include"enemyai/EnemyAI.h"
#include"enemyai/EnemyAction.h"
#include"enemyai/EnemyRound.h"

#include<Windows.h>
#include"GL/glew.h"

#include"cards/PlayerRound.h"


Director::Director(Level& level, std::function<void(DirectorEvent)> onEvent)
	: level(level), action(nullptr), enemyAi(level), onEvent(onEvent)
{
	enemyRound = vnew EnemyRound();
	enemyRound->AddAction(vnew EnemyMove(level, 2));
	enemyRound->AddAction(vnew EnemyAttack(level, 2, 1));

	enemyAi.SetActor(level.ActorById(2));
	enemyAi.SetRoundActions(enemyRound);

	playerRound = nullptr;
}

Director::~Director()
{
	if( enemyRound )
		delete enemyRound;

	if (action)
		delete action;

	if (playerRound)
		delete playerRound;
}

void Director::Update(const InputService& input)
{
	if( !initiativeTracker.EnemyTurn() )
		PlayerTurn(input);
}

void Director::Render()
{
	glPushMatrix();
	if (action)
		action->Render();

	if( enemyRound )
		enemyRound->Render();
	glPopMatrix();
}

void Director::RenderUI(const TextService& text)
{
	glPushMatrix();

	glTranslatef(5.0f, 300.0f, 0.0f);
	if(initiativeTracker.EnemyTurn()) {

		if( enemyRound != nullptr )
			enemyRound->RenderRoundCard(text);
	}
	else {
		if (playerRound == nullptr || playerRound->Finished()){}
		else {
			playerRound->Render(text);
		}
	}
	glPopMatrix();

	if (action != nullptr) {
		text.Print(200, 25, action->Description(), 20, Colorf(1, 1, 1), false, true);
	}
}

void Director::StartRound()
{
	initiativeTracker.CalculateRoundOrder(level);
	NextActor();
}

void Director::PlayerTurn(const InputService& input)
{
	if (action == nullptr || playerRound->Finished() )
		return;

	if ( input.KeyOnce(VK_RETURN)) {
		if (action->Perform(*level.GetPlayer())) {
			
			action->Reset();

			playerRound->Next();
			action = playerRound->GetAction();

			if( action != nullptr )
				action->Highlight();

			if (playerRound->Finished()) {
				delete playerRound;
				playerRound = nullptr;
				level.ClearHighlights();
			}
		}
	}
	else if (input.KeyOnce(VK_LBUTTON)) {
		if (level.HasHoverTarget())
			action->Click(level.GetHoverTarget().Location());
	}
	else if (input.KeyOnce(VK_BACK)) {
		action->Undo();
	}

	/* Flag if player CAN move back, ie attack has happened */
	//if (action != nullptr && input.KeyOnce(VK_ESCAPE)) {
	//	action->Reset();
	//	delete action;
	//	action = nullptr;
	//	level.ClearHighlights();
	//}
}

void Director::EndPlayerTurn() 
{
	if (initiativeTracker.EnemyTurn())
		return;

	if (action == nullptr) {
		NextActor();
	}
}

void Director::SetPlayerRound(PlayerRound* playerRound)
{
	if (this->playerRound) {
		delete this->playerRound;
		this->playerRound = nullptr;
	}

	this->playerRound = playerRound;
	action = playerRound->GetAction();
	action->Highlight();
}

void Director::SetPlayerRound()
{
	std::vector<Action*> playerActions;
	playerActions.push_back(new ActionShieldSelf(level, *level.GetPlayer(), 1));
	playerActions.push_back(new ActionMove(level, *level.GetPlayer(), 2));
	playerActions.push_back(new ActionAttack(level, *level.GetPlayer(), 1, 4, 1));
	playerRound = vnew PlayerRound(playerActions);

	action = playerRound->GetAction();
	action->Highlight();
}

void Director::AdvanceEnemy()
{
	if (initiativeTracker.EnemyTurn()) {
		enemyAi.Step();

		if (enemyAi.Finished()) {
			NextActor();
		}
	}
}

void Director::NextActor()
{
	auto a = initiativeTracker.NextActor();

	if (a == nullptr) {
		onEvent(DirectorEvent::EndOfRound);
	}
	else if (initiativeTracker.EnemyTurn()) {
		onEvent(DirectorEvent::EnemyTurn);
		enemyAi.SetActor(a);
		enemyAi.SetRoundActions(enemyRound);
	}
	else {
		onEvent(DirectorEvent::PlayerTurn);
	}
}
