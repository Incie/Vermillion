#pragma once

#include<glm/glm.hpp>
#include<vector>
#include<string>
#include"../../Framework/services.h"
#include"../Level/Hexagon.h"
#include"StatusEffect.h"

enum class EnemyType {
	Normal,
	Elite,
	Boss,
	SpawnedNormal,
	SpawnedElite,
	Summon
};



std::string StatusEffectToString(StatusEffect statusEffect);
Texture* StatusEffectToTexture(StatusEffect statusEffect);


struct entityattributes {
	std::string name;
	int entityId;
};

struct actorattributes {
	int health;
	int range;
	int shield;
	int retaliate;
	int team;
	int attack;
	int move;
	int initiative;
	int pierce;
};

struct playerattributes {
	int playerId;
	std::string playerName;
};

struct enemyattributes {
	int enemyId;
	EnemyType enemyType;
};


class Entity {
public:
	Entity();
	virtual ~Entity();

	int EntityId() { return entityId; }

	void Setup(const entityattributes& entityattr);
	void SetPosition(const glm::ivec3& tilePosition, const glm::vec3& worldPosition);

	Hexagon& RenderModel() { return renderModel; }
	void SetRenderModel(Hexagon& hex);
	virtual void Render(const TextService& text);
	virtual void PrintStats(const TextService& text) {};


	bool Active() { return active; }
	void Deactivate() { active = false; }
	const std::string& Name() const { return name; }
	const glm::ivec3& Position() const { return positionTile; }
	const glm::vec3 WorldPosition() const { return positionWorld; }

protected:
	bool active;

	std::string name;
	int entityId;
	glm::ivec3 positionTile;
	glm::ivec3 positionWorld;

	Hexagon renderModel;
};

class Actor : public Entity {
public:
	Actor();
	virtual ~Actor();

	static const Actor& NeutralActor();

	void Setup(const actorattributes& actorattr, const entityattributes& entityattr);

	int DoDamage(int attackDamage, int pierce);
	int DoHealing(int healAmount);

	int Attack() const { return attack; }
	int Range() const { return range; }
	int Move() const { return move; }
	int Team() const { return team; }
	int Health() const { return health; }
	int Shield() const { return shield; }
	int Pierce() const { return pierce; }
	int Retaliate() const { return retaliate; }

	void ModifyHealth(int mod) { health += mod; }
	void ModifyShield(int mod) { shield += mod; }
	void ModifyPierce(int mod) { pierce += mod; }
	void ModifyRetaliate(int mod) { retaliate += mod; }

	void AddEndOfTurnAction(int rounds, std::function<void(Actor*)> func) {
		endOfTurnAction.emplace_back( std::pair<int, std::function<void(Actor*)>>(rounds, func) );
	}

	void EndofTurnActions() {
		for(auto iteratorEndOfTurnAction = endOfTurnAction.begin(); iteratorEndOfTurnAction != endOfTurnAction.end(); ) {
			auto& endOfTurnPair = *iteratorEndOfTurnAction;
			auto& turnCount = endOfTurnPair.first;
			auto& callback = endOfTurnPair.second;
			
			turnCount--;
			if(turnCount <= 0) {
				callback(this);
				iteratorEndOfTurnAction = endOfTurnAction.erase(iteratorEndOfTurnAction);
				continue;
			}			

			++iteratorEndOfTurnAction;
		}
	}

	void AddEndOfRoundAction(std::function<void(Actor*)> func) {
		endOfRoundActions.push_back(func);
	}

	void EndOfRoundActions() {
		for (auto endofroundAction : endOfRoundActions)
			endofroundAction(this);
		endOfRoundActions.clear();
	}

	void Initiative(int initiative) { this->initiative = initiative; }
	int Initiative() { return initiative; }

	virtual void PrintStats(std::vector <std::string> &text) const;
	virtual void Render(const TextService& text) override;

	void AddStatus(StatusEffect status);
	void RemoveStatus(StatusEffect status);
	bool HasStatus(StatusEffect status) const;
	bool Stunned() const;
	bool Wounded() const;
	bool Poisoned() const;
	bool Muddled() const;
	bool Strengthened() const;
	bool Disarmed() const;
	bool Immobilised() const;

	bool HasMoved() const { return hasMoved; }
	void Moved() { hasMoved = true; }
	void MovedReset() { hasMoved = false; }

protected:
	int team;
	int maxhealth;
	int health;
	int range;
	int attack;
	int shield;
	int retaliate;
	int move;
	int initiative;
	int pierce;

	bool hasMoved;

	std::vector<StatusEffect> innateEffects;
	std::vector<StatusEffect> immunities;
	std::vector<StatusEffect> statusEffects;

	std::vector<std::pair<int, std::function<void(Actor*)>>> endOfTurnAction;
	std::vector<std::function<void(Actor*)>> endOfRoundActions;
};


class Player : public Actor {
public:
	Player();
	virtual ~Player();

	void Setup(const playerattributes& playerattr, const actorattributes& actorattr, const entityattributes& entityattr);
	virtual void PrintStats(std::vector<std::string>& text) const;


	const std::string& PlayerName() { return playerName; }
protected:
	int playerId;
	std::string playerName;


	//OnHitEffects [xp on hit] [end of turn, or limited]
	//OnPreHit / OnPostHit
	//PlayerDeck
	//ActiveCards
	//Items
};

class Enemy : public Actor {
public:
	Enemy();
	virtual ~Enemy();

	void Setup(const enemyattributes& enemyattr, const actorattributes& actorattr, const entityattributes& entityattr);
	virtual void PrintStats(std::vector<std::string>& text) const;

	int EnemyId() { return enemyId; }
	bool Elite() { 
		if(enemyType == EnemyType::Elite)
			return true;
		return false; 
	}
protected:
	int enemyId;
	EnemyType enemyType;
};