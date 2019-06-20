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
#include"enemyai/actions/EnemyAction.h"
#include"enemyai/EnemyRound.h"

#include<Windows.h>
#include"GL/glew.h"

#include"cards/PlayerRound.h"


Director::Director(Level& level, std::function<void(DirectorEvent)> onEvent)
	: level(level), action(nullptr), onEvent(onEvent), directorStatus(DirectorStatus::EndOfRound), monsterCardDecks(level), spawner(level), activeActor(nullptr)
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

	for(auto e : level.Enemies())
		e->MovedReset();

	initiativeTracker.CalculateRoundOrder(level);
	NextActor();
}

void Director::PlayerTurn(const InputService& input)
{
	if (action == nullptr || playerRound->Finished() )
		return;

	if ( input.KeyOnce(VK_RETURN)) {
		if (action->Perform(*this, *level.GetPlayer())) {
			
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
			action->Click(*this, level.GetHoverTarget().Location());
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
		enemyAi.Step(*this, level);

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
	
	auto activeEnemyRound = monsterDeck->Active();
	
	auto enemyRoundText = std::vector<std::string>();

	if(activeEnemyRound == nullptr)
		return std::pair<int, std::vector<std::string>>(-1, enemyRoundText);

	activeEnemyRound->ToString(enemyRoundText);
	return std::pair<int, std::vector<std::string>>(activeEnemyRound->Initiative(), enemyRoundText);
}

void Director::NextActor()
{
	if(activeActor != nullptr) {
		activeActor->EndofTurnActions();
	}

	activeActor = initiativeTracker.NextActor();

	if (activeActor == nullptr) {
		directorStatus = DirectorStatus::EndOfRound;
		onEvent(DirectorEvent::EndOfRound);
		monsterCardDecks.EndOfRound();
		initiativeTracker.ClearInitiatives();
	}
	else if (initiativeTracker.EnemyTurn()) {
		enemyAi.SetActor(activeActor);

		auto monsterDeck = this->monsterCardDecks.GetMonsterDeck(activeActor->Name());
		auto er = monsterDeck->Active();
		er->Reset();
		er->SetActorStats(*activeActor);

		enemyAi.SetRoundActions(er);
		onEvent(DirectorEvent::EnemyTurn);
	}
	else {
		onEvent(DirectorEvent::PlayerTurn);
	}
}


void Director::OnSpawnRoom()
{
	for(auto enemy : level.Enemies()) {
		auto monsterDeck = monsterCardDecks.GetMonsterDeck(enemy->Name());

		if(monsterDeck->Active() == nullptr) {
			monsterDeck->Draw();
		}

		enemy->Initiative(monsterDeck->Active()->Initiative());
	}

	initiativeTracker.CalculateRoundOrder(level);
	initiativeTracker.ResetOnNextActor();
}

void Director::PerformAttack(int baseDamage, int range, std::vector<StatusEffect> statusEffects, Actor& attacker, ModifierDeck& attackerDeck, Actor& victim)
{
	Tile& vtile = level.TileAt(victim.Position());
	int attackDistance = vtile.DistanceTo(attacker.Position());

	int poisonDamage = 0;
	if(victim.Poisoned())
		poisonDamage = 1;

	int calculatedDamage = baseDamage + poisonDamage;
	auto modifierDraw = attackerDeck.Draw();

	bool disadvantage = false;
	bool advantage = false;

	if(attacker.Muddled()) {
		disadvantage = true;
		level.combatLog.push_back(fmt::format("{0} is Muddled", attacker.Name()));
	}
	if(attacker.Strengthened()) {
		advantage = true;
		level.combatLog.push_back(fmt::format("{0} is Strengthened", attacker.Name()));
	}

	if(range > 1 && attackDistance == 1) {
		disadvantage = true;
		level.combatLog.push_back(fmt::format("{0} is [disadvantaged] using ranged attack in melee", attacker.Name()));
	}

	//if attacker is doing ranged in melee
		//disadvantage = true

	if(disadvantage && advantage)
		level.combatLog.push_back(fmt::format("{0} has Advantage and Disadvantage, they cancel out.", attacker.Name()));


	if(disadvantage != advantage) {
		auto modifierDraw2 = attackerDeck.Draw();

		level.combatLog.push_back(fmt::format("{0} drew {1}", attacker.Name(), Modifier::ToString(modifierDraw.modifiers)));
		level.combatLog.push_back(fmt::format("{0} drew {1}", attacker.Name(), Modifier::ToString(modifierDraw2.modifiers)));

		bool secondDrawIsBetter = modifierDraw2.TotalDamage(calculatedDamage).first > modifierDraw.TotalDamage(calculatedDamage).first;

		if(secondDrawIsBetter && advantage) {
			modifierDraw = modifierDraw2;
			level.combatLog.push_back("Second draw was chosen");
		}
		else {
			level.combatLog.push_back("First draw was chosen");
		}
	}


	for(auto m : modifierDraw.modifiers)
		calculatedDamage = m.ModifyValue(calculatedDamage);

	int pierce = attacker.Pierce();

	for(auto status : statusEffects) {
		if(status == StatusEffect::Pierce) pierce += 1;
		if(status == StatusEffect::Pierce2) pierce += 2;
		if(status == StatusEffect::Pierce3) pierce += 3;
	}


	for(auto& m : modifierDraw.modifiers) {
		if(m.status != ModifierStatus::None) {
			switch(m.status) {
			case ModifierStatus::ShieldSelf1: {
				attacker.ModifyShield(1);
				attacker.AddEndOfRoundAction([](Actor* a) { a->ModifyShield(-1); });
				break;
			}
			case ModifierStatus::Pierce3: {
				pierce += 3;
				break;
			}
			default: {
				switch(m.status) {
				case ModifierStatus::Poison:	  statusEffects.push_back(StatusEffect::Poison);	 break;
				case ModifierStatus::Wound: 	  statusEffects.push_back(StatusEffect::Wound);		 break;
				case ModifierStatus::Stun: 		  statusEffects.push_back(StatusEffect::Stunned);	 break;
				case ModifierStatus::Disarm:	  statusEffects.push_back(StatusEffect::Disarmed);	 break;
				case ModifierStatus::Immobilize:  statusEffects.push_back(StatusEffect::Immobilized); break;
				case ModifierStatus::Strengthen:  statusEffects.push_back(StatusEffect::Strengthen); break;
				case ModifierStatus::Invisible:	  statusEffects.push_back(StatusEffect::Invisible);	 break;
				case ModifierStatus::Muddle: 	  statusEffects.push_back(StatusEffect::Muddle);	 break;
				default:
				break;
				}
				break;
			}
			}
		}
	}

	int actualDamage = victim.DoDamage(calculatedDamage, pierce);

	auto attackerAsPlayer = dynamic_cast<Player*>(&attacker);
	auto attackerName = attackerAsPlayer == nullptr ? attacker.Name() : attackerAsPlayer->PlayerName();

	auto victimAsPlayer = dynamic_cast<Player*>(&victim);
	auto victimName = victimAsPlayer == nullptr ? victim.Name() : victimAsPlayer->PlayerName();

	level.combatLog.push_back(fmt::format("{0} did {1} ({4}[Base] + {3}) damage to {2}", attackerName, actualDamage, victimName, Modifier::ToString(modifierDraw.modifiers), baseDamage));

	for(auto st : statusEffects) {
		switch(st) {
		case StatusEffect::Strengthen: attacker.AddStatus(st); break;
		case StatusEffect::Invisible: attacker.AddStatus(st); break;
		case StatusEffect::Pierce:
		case StatusEffect::Pierce2:
		case StatusEffect::Pierce3:
		break;
		default: {
			level.combatLog.push_back(fmt::format("{1} was {0}", StatusEffectToString(st), victim.Name()));
			victim.AddStatus(st);
			break;
		}
		}
	}

	if(victim.Health() <= 0) {
		level.combatLog.push_back(fmt::format("{0} died", victim.Name()));

		if(dynamic_cast<Enemy*>(&victim) != nullptr) /* todo: and not an enemy-spawned enemy*/
			spawner.SpawnCoinAt(victim.Position());

		level.RemoveActorFromTileById(victim.EntityId());
		victim.Deactivate();
	}
	else {
		if(victim.Retaliate() > 0) {
			// if( health - retaliate <= 0 )
				// query what to do

			auto retaliate = victim.Retaliate();
			attacker.ModifyHealth(-retaliate);
			level.combatLog.push_back(fmt::format("{0} retaliated with {1} damage", victim.Name(), retaliate));
		}
	}
}