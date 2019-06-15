#include"pch.h"
#include "Director.h"
#include"../services.h"

#include"cards/MonsterDeck.h"

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
	: level(level), action(nullptr), onEvent(onEvent), directorStatus(DirectorStatus::EndOfRound), monsterCardDecks(level)
{
	playerRound = nullptr;
}

Director::~Director()
{
	if (action)
		delete action;

	if (playerRound)
		delete playerRound;
}

void Director::Initialize()
{
	for(auto enemy : level.Enemies()) {
		monsterCardDecks.GetMonsterDeck(enemy->Name());
	}
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
	
	glPopMatrix();

	glPushMatrix();
	if( initiativeTracker.EnemyTurn() )
		enemyAi.Render();
	glPopMatrix();
}

void Director::RenderUI(const TextService& text)
{
	glPushMatrix();

	glTranslatef(5.0f, 300.0f, 0.0f);
	if(initiativeTracker.EnemyTurn()) {
	}
	else {
		if (playerRound == nullptr || playerRound->Finished()){}
		else {
			playerRound->Render(text);
		}
	}
	glPopMatrix();

	if (action != nullptr) {
		text.Print(200, 25, action->Description(), 20, Colors::White, false, true);
	}
}

void Director::StartRound()
{
	directorStatus = DirectorStatus::RoundStarted;

	monsterCardDecks.DrawAll();

	for(auto enemy : level.Enemies() ) {
		auto er = monsterCardDecks.GetMonsterDeck(enemy->Name());
		enemy->Initiative(er->Active()->Initiative());
	}

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

void Director::AdvanceEnemy()
{
	if (initiativeTracker.EnemyTurn()) {
		enemyAi.Step(level);

		if (enemyAi.Finished()) {
			NextActor();
		}
		else {
			onEvent(DirectorEvent::AdvanceEnemy);
		}
	}
}



//TODO: Utilize render2texture instead?
std::pair<int, std::vector<std::string>> Director::GetEnemyRound(const std::string& name)
{
	MonsterDeck* monsterDeck = nullptr;
	if(name == "") 
		monsterDeck = this->monsterCardDecks.GetMonsterDeck(enemyAi.GetActor()->Name());
	else 
		monsterDeck = this->monsterCardDecks.GetMonsterDeck(name);
	auto er = monsterDeck->Active();
	
	auto enemyRoundText = std::vector<std::string>();
	er->ToString(enemyRoundText);
	return std::pair(er->Initiative(), enemyRoundText);
}



void Director::NextActor()
{
	auto a = initiativeTracker.NextActor();

	if (a == nullptr) {
		directorStatus = DirectorStatus::EndOfRound;
		onEvent(DirectorEvent::EndOfRound);
	}
	else if (initiativeTracker.EnemyTurn()) {
		enemyAi.SetActor(a);

		auto monsterDeck = this->monsterCardDecks.GetMonsterDeck(a->Name());
		auto er = monsterDeck->Active();
		er->Reset();

		enemyAi.SetRoundActions(er);
		onEvent(DirectorEvent::EnemyTurn);
	}
	else {
		onEvent(DirectorEvent::PlayerTurn);
	}
}
