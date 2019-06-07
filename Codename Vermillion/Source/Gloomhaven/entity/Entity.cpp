#include"pch.h"
#include "Entity.h"
#include"fmt/format.h"

Entity::Entity()
	: renderModel(), active(true)
{
}

Entity::~Entity()
{
}

void Entity::Setup(const entityattributes& entityattr)
{
	entityId = entityattr.entityId;
	name = entityattr.name;

}

void Entity::SetPosition(const glm::ivec3& tilePosition, const glm::vec3& worldPosition)
{
	positionTile = tilePosition;
	positionWorld = worldPosition;
}

void Entity::SetRenderModel(Hexagon& hex)
{
	renderModel = hex;
}

#include"GL/glew.h"
void Entity::Render(const TextService& text)
{
	glPushMatrix();
		glTranslatef((float)positionWorld.x, (float)positionWorld.y, (float)positionWorld.z);
		renderModel.Render();
	glPopMatrix();
}

Actor::Actor()
{
}

Actor::~Actor()
{
}

void Actor::Setup(const actorattributes& actorattr, const entityattributes& entityattr)
{
	Entity::Setup(entityattr);
	team = actorattr.team;
	health = actorattr.health;
	maxhealth = actorattr.health;
	range = actorattr.range;
	attack = actorattr.attack;
	shield = actorattr.shield;
	retaliate = actorattr.retaliate;
	move = actorattr.move;
	initiative = actorattr.initiative;
}

int Actor::DoDamage(int attackDamage)
{
	auto actualDamage = attackDamage - shield;

	//if( isPoisoned ) actualDamage += 1
	if (actualDamage < 0) 
		actualDamage = 0;

	health -= actualDamage;

	return actualDamage;
}


void Actor::PrintStats(const TextService& text)
{
	text.Print(0, 0, fmt::format("Health: {0} / {1}", health, maxhealth), 16, Colors::White, false, true);

	if (move != 0)
		text.Print(0, 0, fmt::format("Move: {0}", move), 16, Colors::White, false, true);

	if (attack != 0)
		text.Print(0, 0, fmt::format("Attack: {0}", attack), 16, Colors::White, false, true);

	if (range != 0)
		text.Print(0, 0, fmt::format("Range: {0}", range), 16, Colors::White, false, true);

	if (shield != 0) 
		text.Print(0, 0, fmt::format("Shield: {0}", shield), 16, Colors::White, false, true);

	if( retaliate != 0 )
		text.Print(0, 0, fmt::format("Retaliate: {0}", retaliate), 16, Colors::White, false, true);
}

void Actor::Render(const TextService& text)
{
	Entity::Render(text);

	text.PrintCenter(positionWorld.x, positionWorld.y, fmt::format("H:{}", health), 12, Colors::Red);

	if( shield > 0 )
		text.PrintCenter(positionWorld.x, positionWorld.y+12.0f, fmt::format("S:{}", shield), 12, glm::vec3(0.6f, 0.6f, 0.6f));
}

Player::Player()
{
}

Player::~Player()
{
}

void Player::Setup(const playerattributes& playerattr, const actorattributes& actorattr, const entityattributes& entityattr)
{
	Actor::Setup(actorattr, entityattr);
	playerId = playerattr.playerId;
	playerName = playerattr.playerName;
}

void Player::PrintStats(const TextService& text)
{
	text.Print(0, 0, playerName, 18, Colors::White, false, true);
	Actor::PrintStats(text);
}

Enemy::Enemy()
{
}

Enemy::~Enemy()
{
}

void Enemy::Setup(const enemyattributes& enemyattr, const actorattributes& actorattr, const entityattributes& entityattr)
{
	Actor::Setup(actorattr, entityattr);
	enemyId = enemyattr.enemyId;
	enemyType = enemyattr.enemyType;
}

void Enemy::PrintStats(const TextService& text)
{
	text.Print(0, 0, fmt::format("[{2}]{0} ({1})", name, enemyType == EnemyType::Elite ? "Elite" : "Normal", enemyId), 16, Colors::White, false, true);
	Actor::PrintStats(text);
}
