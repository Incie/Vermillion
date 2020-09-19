#include"pch/pch.h"
#include"EnemyShield.h"
#include"../../entity/Entity.h"
#include"../../level/Level.h"
#include"../../icons/icons.h"
#include"render.h"

EnemyHealSelf::EnemyHealSelf(int healAmount)
	: EnemyAction(), heal(healAmount)
{
	actionDescription = fmt::format("Heal self {0}", healAmount);
}

bool EnemyHealSelf::CanPerform(const Actor& actor)
{
	if(actor.Stunned())
		return false;
	return true;
}

void EnemyHealSelf::Calculate(Level& level, const Actor& actor)
{
	calculated = true;
	actorWorldPosition = actor.WorldPosition();
}

void EnemyHealSelf::Perform(Director& director, Level& level, Actor& actor)
{
	auto healedFor = actor.DoHealing(heal);
	level.combatLog.push_back(fmt::format("{0} healed for {1}", actor.Name(), healedFor));
}

void EnemyHealSelf::Render()
{
	if(!calculated)
		return;

	auto texture = Icons::Get("heal");
	Render::Quad(actorWorldPosition, glm::vec2(15, 15), glm::vec3(0, 1, 0));
}


EnemyShieldSelf::EnemyShieldSelf(int shield)
	: EnemyAction(), shield(shield)
{
	actionDescription = fmt::format("Shield self {0}", shield);
}

bool EnemyShieldSelf::CanPerform(const Actor& actor)
{
	return !actor.Stunned();
}

void EnemyShieldSelf::Calculate(Level& level, const Actor& actor)
{
	actorWorldPosition = actor.WorldPosition();
	calculated = true;
}

void EnemyShieldSelf::Perform(Director& director, Level& level, Actor& actor)
{
	actor.ModifyShield(shield);
	int s = shield;
	actor.AddEndOfRoundAction([s](Actor* a) { a->ModifyShield(-s); });
}

void EnemyShieldSelf::Render()
{
	if(!calculated)
		return;

	auto texture = Icons::Get("shield");
	Render::Quad(actorWorldPosition, glm::vec2(15, 15), *texture, glm::vec3(0, 1, 0), true);
}

EnemyRetaliate::EnemyRetaliate(int retaliate)
	: EnemyAction(), retaliate(retaliate)
{
	actionDescription = fmt::format("Retaliate {0}", retaliate);
}

bool EnemyRetaliate::CanPerform(const Actor& actor)
{
	return !actor.Stunned();
}

void EnemyRetaliate::Calculate(Level& level, const Actor& actor)
{
	calculated = true;
	actorWorldPosition = actor.WorldPosition();
}

void EnemyRetaliate::Perform(Director& director, Level& level, Actor& actor)
{
	actor.ModifyRetaliate(retaliate);
	int r = retaliate;
	actor.AddEndOfRoundAction([r](Actor* a) { a->ModifyRetaliate(-r); });
}

void EnemyRetaliate::Render()
{
	if(!calculated)
		return;

	auto texture = Icons::Get("retaliate");
	Render::Quad(actorWorldPosition, glm::vec2(15, 15), *texture, glm::vec3(1, 0, 0), true);
}

EnemySelfCast::EnemySelfCast(StatusEffect effect)
	: effect(effect)
{
	actionDescription = StatusEffectToString(effect);
}

bool EnemySelfCast::CanPerform(const Actor& actor)
{
	return !actor.Stunned();
}

void EnemySelfCast::Calculate(Level& level, const Actor& actor)
{
	actorWorldPosition = actor.WorldPosition();
	calculated = true;
}

void EnemySelfCast::Perform(Director& director, Level& level, Actor& actor)
{
	actor.AddStatus(effect);
	auto statusEffectCopy = effect;
	actor.AddEndOfTurnAction(2, [statusEffectCopy](Actor* a) {
		a->RemoveStatus(statusEffectCopy);
	});
}

void EnemySelfCast::Render()
{
	if(!calculated)
		return;

	auto texture = Icons::Get("strengthen");
	Render::Quad(actorWorldPosition, glm::vec2(25, 25), *texture, Colors::White, true);
}
