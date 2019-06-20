#include"pch.h"
#include"MonsterCardGenerator.h"
#include"../enemyai/actions/EnemyAction.h"
#include"../enemyai/actions/EnemyMove.h"
#include"../enemyai/actions/EnemyAttack.h"
#include"../enemyai/actions/EnemyShield.h"
#include"MonsterDeck.h"

MonsterCardGenerator::MonsterCardGenerator(Level& level)
	: level(level)
{
}

MonsterCardGenerator::~MonsterCardGenerator()
{
	for( auto monsterDeckPair : monsterDeckMap ){
		auto enemyRound = monsterDeckPair.second;
		delete enemyRound;
	}

	monsterDeckMap.clear();
}

MonsterDeck* MonsterCardGenerator::GetMonsterDeck(const std::string& entityName)
{
	auto it = monsterDeckMap.find(entityName);

	if(it == monsterDeckMap.end()) {
		LoadDeck(entityName);
		return monsterDeckMap[entityName];
	}

	return it->second;
}

void MonsterCardGenerator::DrawAll()
{
	for(auto it : monsterDeckMap) {
		it.second->Draw();
	}
}

void MonsterCardGenerator::Reshuffle()
{
	for(auto it : monsterDeckMap) {
		if(it.second->Active() != nullptr) {
			if( it.second->Active()->Reshuffle() )
				it.second->Shuffle();
		}
	}
}

void MonsterCardGenerator::EndOfRound()
{
	for(auto kv : monsterDeckMap) {
		auto monsterDeck = kv.second;
		
		auto active = monsterDeck->Active();
		if(active != nullptr) {
			if(active->Reshuffle()) {
				monsterDeck->Shuffle();
			}

			monsterDeck->Reset();
		}
	}
}

bool MonsterCardGenerator::LoadDeck(const std::string& entityName)
{
	if( monsterDeckMap.find(entityName) != monsterDeckMap.end() )
		return;

	if(entityName == "Bandit Guard") {
		auto md = vnew MonsterDeck(entityName, std::vector<EnemyRound*>{
			//vnew EnemyRound{ 30, {vnew EnemyMove(1), vnew EnemyAttack(-1, 0)} },
			//vnew EnemyRound{ 50, {vnew EnemyMove(0), vnew EnemyAttack(0, 0)} },
			//vnew EnemyRound{ 15, {vnew EnemyShieldSelf(1), vnew EnemyRetaliate(1) }, true},
			//vnew EnemyRound{ 70, {vnew EnemyMove(-1), vnew EnemyAttack(1, 0)} },
			//vnew EnemyRound{ 35, {vnew EnemyMove(-1), vnew EnemyAttack(0, 1)} },
			vnew EnemyRound{ 15, {vnew EnemyShieldSelf(1), vnew EnemyAttack(0, 0, 1, {StatusEffect::Poison})} },
			vnew EnemyRound{ 50, {vnew EnemyMove(0), vnew EnemyAttack(0, 0)} },
			vnew EnemyRound{ 55, {vnew EnemyMove(-1), vnew EnemyAttack(0, 0), vnew EnemySelfCast(StatusEffect::Strengthen)} }
		});
		md->Shuffle();
		monsterDeckMap[entityName] = md;
	}
	else if(entityName == "Living Bones") {
		auto md = vnew MonsterDeck(entityName, std::vector<EnemyRound*>{
			vnew EnemyRound{ 45, {vnew EnemyMove(0), vnew EnemyAttack(0, 0)} },
			vnew EnemyRound{ 12, {vnew EnemyShieldSelf(1), vnew EnemyHealSelf(2)}, true },
			vnew EnemyRound{ 64, {vnew EnemyMove(-1), vnew EnemyAttack(1, 0)} },
			vnew EnemyRound{ 45, {vnew EnemyMove(0), vnew EnemyAttack(0, 0)} },
			vnew EnemyRound{ 25, {vnew EnemyMove(1), vnew EnemyAttack(-1, 0)} },
			vnew EnemyRound{ 81, {vnew EnemyAttack(2, 0)} },
			vnew EnemyRound{ 74, {vnew EnemyMove(0), vnew EnemyAttack(0, 0, 1), vnew EnemyAttack(0, 0, 1), vnew EnemyAttack(0, 0, 1)} },
			vnew EnemyRound{ 20, {vnew EnemyMove(-2), vnew EnemyAttack(0, 0), vnew EnemyHealSelf(2)}, true }
		});
		md->Shuffle();
		monsterDeckMap[entityName] = md;
	}
	else if(entityName == "Bandit Archer") {
		auto md = vnew MonsterDeck(entityName, std::vector<EnemyRound*>{
			vnew EnemyRound{ 16, {vnew EnemyMove(1), vnew EnemyAttack(-1, 0)} },
			vnew EnemyRound{ 32, {vnew EnemyMove(0), vnew EnemyAttack(1, -1)} },
			vnew EnemyRound{ 14, {vnew EnemyMove(-1), vnew EnemyAttack(-1, 0)}/* create trap */ },
			vnew EnemyRound{ 56, {vnew EnemyAttack(1, -1, 2)} },
			vnew EnemyRound{ 68, {vnew EnemyAttack(1, 1)}, true },
			vnew EnemyRound{ 31, {vnew EnemyMove(0), vnew EnemyAttack(0, 0)} },
			vnew EnemyRound{ 29, {vnew EnemyMove(0), vnew EnemyAttack(-1, 1, 1, {StatusEffect::Immobilized})}, true },
			vnew EnemyRound{ 44, {vnew EnemyMove(-1), vnew EnemyAttack(1, 0)} }
		});
		md->Shuffle();
		monsterDeckMap[entityName] = md;
	}
	else
		throw "unknown entityName in MonsterCardGenerator";

	return true;
}
