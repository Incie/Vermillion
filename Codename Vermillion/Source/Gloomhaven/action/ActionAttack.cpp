#include"pch.h"
#include"ActionAttack.h"
#include"../level/Level.h"
#include"../entity/Entity.h"
#include"..//Director.h"

ActionAttack::ActionAttack(Level& level, Actor& actor, int range, int attackDamage, int attacks, std::vector<StatusEffect> statusEffects)
	: Action(level, actor), range(range), attacks(attacks), baseDamage(attackDamage)
{
	this->statusEffects.insert(this->statusEffects.end(), statusEffects.begin(), statusEffects.end());
	actionDescription = fmt::format("Attack {0} Range {1} Target {2}", attackDamage, range, attacks);
	Highlight();
}

ActionAttack::~ActionAttack()
{
	marks.clear();
}

void ActionAttack::Click(Director& director, const glm::ivec3& target)
{
	const Tile& tile = level.TileAt(target);

	if (tile.IsOccupied()) {

		if (IsMarked(tile.OccupiedId())){
			//todo RemoveMark
			return;
		}

		if (marks.size() >= attacks)
			return;

		if (tile.DistanceTo(target) > range)
			return;

		if (level.TileAt(actor.Position()).DistanceTo(target) == 0)
			return;

		marks.push_back(target);
		Highlight();
	}
}

void ActionAttack::Undo()
{
	if (marks.size() > 0)
		marks.pop_back();

	Highlight();
}

void ActionAttack::Reset()
{
	marks.clear();
	Highlight();
}

bool ActionAttack::Perform(Director& director, Actor& actor)
{
	for (auto& mark : marks) {
		auto& tile = level.TileAt(mark);
		auto targetActor = level.ActorById(tile.OccupiedId());

		director.PerformAttack(baseDamage, range, statusEffects, actor, level.playerModifiers, *targetActor);
	}

	return true;
}

#include"GL/glew.h"
void ActionAttack::Render()
{
	const auto& tile0 = level.TileAt(actor.Position());
	glBegin(GL_LINES);
	for (auto mark : marks) {
		glColor3f(1, 0, 0);

		const auto& tile1 = level.TileAt(mark);
		glVertex2fv(&tile0.WorldPosition().x);
		glVertex2fv(&tile1.WorldPosition().x);
	}
	glEnd();
}

void ActionAttack::Highlight()
{
	level.ClearHighlights();

	if (marks.size() >= attacks)
		return;

	level.Highlight(actor.Position(), range, glm::vec3(1, 0, 0), [this](const Tile & tile) {
		if (tile.DistanceTo(actor.Position()) == 0)
			return false;

		if (!tile.IsOccupied())
			return false;

		if (IsMarked(tile.OccupiedId()))
			return false;


		return true; 
	});
}

void ActionAttack::NoHighlight()
{
}

bool ActionAttack::IsMarked(int entityId)
{
	for (auto mark : marks)
	{
		const auto& tile = level.TileAt(mark);
		if (tile.OccupiedId() == entityId)
			return true;
	}

	return false;
}
