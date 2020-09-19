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
	: hasMoved(false)
{
}

Actor::~Actor()
{
}

const Actor& Actor::NeutralActor()
{
	static bool initialized = false;
	static Actor actor;

	if( initialized )
		return actor;

	entityattributes ea;
	ea.entityId = -1;
	ea.name = "NeutralActor";
	actorattributes aa = {0};
	actor.Setup(aa, ea);
	actor.Deactivate();
	initialized = true;
	return actor;
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
	pierce = 0;
}

int Actor::DoDamage(int attackDamage, int pierce)
{
	auto actualDamage = attackDamage - std::max<int>(shield - pierce, 0);

	if (actualDamage < 0) 
		actualDamage = 0;

	health -= actualDamage;

	return actualDamage;
}

int Actor::DoHealing(int healAmount)
{
	if(Poisoned()) {
		healAmount = 0;
		RemoveStatus(StatusEffect::Poison);
	}

	if(Wounded()) {
		RemoveStatus(StatusEffect::Wound);
	}

	health = std::min<int>(health + healAmount, maxhealth);
	return healAmount;
}


void Actor::PrintStats(std::vector<std::string>& text) const
{
	text.emplace_back(fmt::format("Health: {0} / {1}", health, maxhealth));

	if(move != 0) text.emplace_back(fmt::format("Move: {0}", move));
	if(attack != 0) text.emplace_back(fmt::format("Attack: {0}", attack));
	if(range != 0) text.emplace_back(fmt::format("Range: {0}", range));
	if(shield != 0) text.emplace_back(fmt::format("Shield: {0}", shield));
	if(retaliate != 0) text.emplace_back(fmt::format("Retaliate: {0}", retaliate));
	if(pierce != 0) text.emplace_back(fmt::format("Pierce: {0}", pierce));
	if(statusEffects.size() > 0) {
		for(auto status : statusEffects) {
			text.emplace_back(StatusEffectToString(status));
		}
	}
}

#include"..//icons/icons.h"
#include"..//render.h"
#include"..//constants.h"
void Actor::Render(const TextService& text)
{
	Entity::Render(text);

	glDisable(GL_LIGHTING);

	auto iconSize = glm::vec2(20);
	auto fontSize = 16;

	const float stepSize = 6.28f / 6.0f;
	auto healthPosition = glm::vec3(positionWorld.x + cos(stepSize * 4.0f) * 30.0f, positionWorld.y + sin(stepSize * 4.0f) * 30.0f, 0.0f);

	auto healthTexture = Icons::Get("health");
	Render::Quad(healthPosition, iconSize, *healthTexture, Colors::White, true);
	text.Print(healthPosition.x, healthPosition.y - 2, fmt::format("{0}", health), fontSize - 4 * (health > 9 ? 1 : 0), Colors::White, true, false);

	if(shield > 0) {
		auto shieldPosition = glm::vec3(positionWorld.x + cos(stepSize * 5.0f) * 30.0f, positionWorld.y + sin(stepSize * 5.0f) * 30.0f, 0.0f);

		auto shieldTexture = Icons::Get("shield");
		Render::Quad(shieldPosition, iconSize, *shieldTexture, Colors::White, true);
		text.Print(shieldPosition.x, shieldPosition.y - 2, fmt::format("{0}", shield), fontSize, Colors::Black, true, false);
	}

	float step = 0.0f;

	if(retaliate > 0) {
		auto statusPosition = glm::vec3(positionWorld.x + cos(stepSize * step) * 30.0f, positionWorld.y + sin(stepSize * step) * 30.0f, 0.0f);

		auto retaliateTexture = Icons::Get("retaliate");
		Render::Quad(statusPosition, iconSize, *retaliateTexture, Colors::Red, true);
		step += 1.0f;
	}

	if( statusEffects.size() > 0 ){
		glm::vec3 position = positionWorld;
		position.x -= (statusEffects.size() - 1) * (iconSize.x * 0.5f);
		position.y += 5.0f;

		for(auto st : statusEffects) {
			auto statusPosition = glm::vec3(positionWorld.x + cos(stepSize * step) * 30.0f, positionWorld.y + sin(stepSize * step) * 30.0f, 0.0f);

			auto statusTexture = StatusEffectToTexture(st);
			Render::Quad(statusPosition, iconSize, *statusTexture, Colors::White, true);
			step += 1.0f;
		}
	}

	glEnable(GL_LIGHTING);
}

void Actor::AddStatus(StatusEffect status)
{
	if(!HasStatus(status))
		statusEffects.push_back(status);
}

void Actor::RemoveStatus(StatusEffect status)
{
	auto it = std::remove_if(statusEffects.begin(), statusEffects.end(), [status](StatusEffect s) { return s == status; });
	statusEffects.erase(it);
}

bool Actor::HasStatus(StatusEffect status) const
{
	for(auto statusEffect : statusEffects)
	{
		if(statusEffect == status)
			return true;
	}
	return false;
}

bool Actor::Stunned() const
{
	return HasStatus(StatusEffect::Stunned);
}

bool Actor::Wounded() const
{
	return HasStatus(StatusEffect::Wound);
}

bool Actor::Poisoned() const
{
	return HasStatus(StatusEffect::Poison);
}

bool Actor::Muddled() const
{
	return HasStatus(StatusEffect::Muddle);
}

bool Actor::Strengthened() const
{
	return HasStatus(StatusEffect::Strengthen);
}

bool Actor::Disarmed() const
{
	return HasStatus(StatusEffect::Disarmed);
}

bool Actor::Immobilised() const
{
	return HasStatus(StatusEffect::Immobilized);
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

void Player::PrintStats(std::vector<std::string>& text) const
{
	text.emplace_back(playerName);
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

void Enemy::PrintStats(std::vector<std::string>& text) const
{
	text.emplace_back(fmt::format("[{2}]{0} ({1})", name, enemyType == EnemyType::Elite ? "Elite" : "Normal", enemyId));
	Actor::PrintStats(text);
}

std::string StatusEffectToString(StatusEffect statusEffect)
{
	switch(statusEffect) {
		case StatusEffect::Muddle: return "Muddle";
		case StatusEffect::Strengthen: return "Strengthened";
		case StatusEffect::Poison: return "Poisoned";
		case StatusEffect::Wound: return "Wounded";
		case StatusEffect::Immobilized: return "Immobilized";
		case StatusEffect::Disarmed: return "Disarmed";
		case StatusEffect::Stunned: return "Stunned";
		case StatusEffect::Doomed: return "Doomed";
		case StatusEffect::Regenerate: return "Regenerating";
		case StatusEffect::Invisible: return "Invisible";
		case StatusEffect::Pierce: return "Pierce 1";
		case StatusEffect::Pierce2: return "Pierce 2";
		case StatusEffect::Pierce3: return "Pierce 3";
	}

	throw "invalid StatusEffect in StatusEffectToString()";
}

#include"..//icons/icons.h"
Texture* StatusEffectToTexture(StatusEffect statusEffect)
{
	switch(statusEffect) {
		case StatusEffect::Muddle: return	  Icons::Get("muddle");
		case StatusEffect::Strengthen: return Icons::Get("strengthen");
		case StatusEffect::Poison: return 	  Icons::Get("poison");
		case StatusEffect::Wound: return 	  Icons::Get("wound");
		case StatusEffect::Immobilized: return Icons::Get("immobilize"); 
		case StatusEffect::Disarmed: return   Icons::Get("disarm");
		case StatusEffect::Stunned: return 	  Icons::Get("stun");
		/*case StatusEffect::Doomed: return 	  Icons::Get("doomed");*/
		//case StatusEffect::Regenerate: return Icons::Get("regenerate");
		case StatusEffect::Invisible: return  Icons::Get("invisible");
		//case StatusEffect::Pierce: return 	  Icons::Get("");
		//case StatusEffect::Pierce2: return 	  Icons::Get("");
		//case StatusEffect::Pierce3: return 	  Icons::Get("");
	}

	throw "invalid StatusEffect in StatusEffectToTexture()";
}
