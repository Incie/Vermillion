#include"pch.h"
#include "Entity.h"

Entity::Entity()
	: renderModel()
{
}

Entity::~Entity()
{
}

void Entity::Setup(const entityattributes& entityattr)
{
}

void Entity::SetPosition(glm::ivec3& tilePosition, glm::vec3& worldPosition)
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
	range = actorattr.range;
	attack = actorattr.attack;
	shield = actorattr.shield;
	retaliate = actorattr.retaliate;
	move = actorattr.move;
}

#include"fmt/format.h"
void Actor::Render(const TextService& text)
{
	Entity::Render(text);

	text.Print(positionWorld.x, positionWorld.y, fmt::format("H:{}", health), 12, Colorf(1, 0, 0));
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
