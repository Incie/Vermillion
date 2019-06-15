#include"pch.h"
#include"ActionShieldSelf.h"
#include"../entity/Entity.h"
#include"../render.h"
#include"../icons/icons.h"

#include"../level/Level.h"

ActionShieldSelf::ActionShieldSelf(Level& level, Actor& actor, int shield)
	: Action(level, actor), shield(shield)
{
	actionDescription = fmt::format("Shield {0}", shield);
}

ActionShieldSelf::~ActionShieldSelf()
{
}

void ActionShieldSelf::Click(const glm::ivec3& target)
{
}

void ActionShieldSelf::Undo()
{
}

void ActionShieldSelf::Reset()
{
}

bool ActionShieldSelf::Perform(Actor& actor)
{
	actor.ModifyShield(shield);
	auto shieldCopy = shield;
	actor.AddEndOfRoundAction([shieldCopy](Actor * a) { a->ModifyShield(-shieldCopy); });
	return true;
}

void ActionShieldSelf::Render()
{
	auto texture = Icons::Get("Shield");
	auto& tile = level.TileAt(actor.Position());

	Render::Quad(tile.WorldPosition(), glm::vec2(20, 20), *texture, Colors::Green, true);
}
