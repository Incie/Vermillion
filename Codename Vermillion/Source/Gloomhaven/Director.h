#pragma once

#include<vector>
#include<string>
#include"enemyai/EnemyAI.h"
#include"cards/MonsterCardGenerator.h"
#include"director/Initiative.h"
#include"entity/Entity.h"
#include"cards/Modifiers.h"
#include"level/Spawner.h"

class InputService;
class TextService;
class Level;
class Action;
class EnemyRound;
class PlayerRound;

enum class DirectorEvent {
	EndOfRound = 1,
	EnemyTurn = 2,
	PlayerTurn = 3,
	AdvanceEnemy = 4
};

enum class DirectorStatus {
	EndOfRound = 1,
	RoundStarted = 2
};

class Director {
public:
	Director(Level& level, std::function<void(DirectorEvent)> onEvent);
	~Director();

	void Initialize();

	void Update(const InputService& input);
	void Render();
	void RenderUI(const TextService& text);

	void StartRound();
	void EndPlayerTurn();

	void SetPlayerRound(PlayerRound* playerRound);
	void AdvanceEnemy();

	std::pair<int, std::vector<std::string>> GetEnemyRound(const std::string& name = "");

	InitiativeTracker& GetInitiativeTracker() { return initiativeTracker; }

	void OnSpawnRoom();

	void PerformAttack(int baseDamage, int range, std::vector<StatusEffect> statusEffects, Actor& attacker, ModifierDeck& attackerDeck, Actor& victim);
	DirectorStatus Status() { return directorStatus; }
private:
	void PlayerTurn(const InputService& input);
	void NextActor();

	DirectorStatus directorStatus;

	Spawner spawner;
	
	InitiativeTracker initiativeTracker;
	
	MonsterCardGenerator monsterCardDecks;
	EnemyAI enemyAi;
	
	PlayerRound* playerRound;
	Action* action;
	
	Level& level;

	std::function<void(DirectorEvent)> onEvent;
};