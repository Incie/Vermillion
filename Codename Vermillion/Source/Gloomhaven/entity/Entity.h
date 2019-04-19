#pragma once

#include<glm/glm.hpp>
#include<vector>
#include<string>
#include"../../Framework/services.h"
#include"../Level/Hexagon.h"

enum class EnemyType {
	Normal,
	Elite,
	Boss,
	SpawnedNormal,
	SpawnedElite,
	Summon
};

enum class StatusEffect {
	Muddle,
	Strengthen,
	Poison,
	Wound,
	Immobilized,
	Disarmed
};

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


	const std::string& Name() const { return name; }
	const glm::ivec3& Position() const { return positionTile; }

protected:
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

	void Setup(const actorattributes& actorattr, const entityattributes& entityattr);

	int DoDamage(int attackDamage);

	int Team() const { return team; }
	int Health() { return health; }

	virtual void PrintStats(const TextService& text);
	virtual void Render(const TextService& text) override;
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

	std::vector<StatusEffect> statusEffects;

	std::vector<int> endOfTurnAction;
	std::vector<int> endOfRoundAction;
};


class Player : public Actor {
public:
	Player();
	virtual ~Player();

	void Setup(const playerattributes& playerattr, const actorattributes& actorattr, const entityattributes& entityattr);
	virtual void PrintStats(const TextService& text);

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
	virtual void PrintStats(const TextService& text);

protected:
	int enemyId;
	EnemyType enemyType;
};