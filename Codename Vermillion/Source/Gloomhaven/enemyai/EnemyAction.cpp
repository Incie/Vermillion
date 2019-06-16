#include"pch.h"
#include"EnemyAction.h"
#include"../entity/Entity.h"
#include"../level/Level.h"
#include"GL/glew.h"

EnemyAction::EnemyAction()
	: state(0)
{
}

EnemyAction::~EnemyAction()
{
	targets.clear();
}

EnemyMove::EnemyMove(int move)
	: EnemyAction(), move(move)
{
	actionDescription = fmt::format("Move {0}", move);
}


bool EnemyMove::CanPerform(const Actor& actor)
{
	if(actor.Immobilised() || actor.Stunned() )
		return false;

	return true;
}

void EnemyMove::Calculate(Level& level, const Actor& actor)
{
	startPoint = level.TileAt(actor.Position()).WorldPosition();
	state = 1;

	const auto player = level.GetPlayer();
	const auto& playerTile = level.TileAt(player->Position());

	const auto& enemyTile = level.TileAt(actor.Position());

	if (playerTile.DistanceTo(enemyTile.Location()) == 1)
		return;

	auto tilesWithinRange = level.TilesWithin(actor.Position(), actor.Move() + move);

	if (tilesWithinRange.size() == 0)
		return;

	int closest = 99;
	for (auto tile : tilesWithinRange) {
		int distanceToTarget = tile->DistanceTo(playerTile.Location());
		if (distanceToTarget < closest)
			closest = distanceToTarget;
	}

	if (closest == 99)
		return;

	std::sort(tilesWithinRange.begin(), tilesWithinRange.end(), [player](auto const x, auto const y) {
		auto d0 = x->DistanceTo(player->Position());
		auto d1 = y->DistanceTo(player->Position());
		return d0 < d1;
		});

	//also consider least movement

	for (auto tile : tilesWithinRange) {
		if (tile->IsOccupied())
			continue;

		targets.push_back(std::pair<glm::ivec3, glm::vec3>(tile->Location(), tile->WorldPosition()));
		break;
	}
}

void EnemyMove::Perform(Level& level, Actor& actor)
{
	level.ClearHighlights();
	auto currentPosition = actor.Position();

	auto moveTargets = Targets();

	if (moveTargets.size() == 1) {
		auto nextPosition = moveTargets[0];
		auto& targetTile = level.TileAt(nextPosition.first);
		auto& startTile = level.TileAt(currentPosition);

		startTile.SetOccupied(-1);
		targetTile.SetOccupied(actor.EntityId());
		actor.SetPosition(targetTile.Location(), targetTile.WorldPosition());
	}
}

std::vector<std::pair<glm::ivec3, glm::vec3>> EnemyAction::Targets()
{
	return targets;
}

void EnemyMove::Render()
{
	for (auto target : targets) {
		glBegin(GL_LINES);
		glVertex2fv(&startPoint.x);
		glVertex2fv(&target.second.x);
		glEnd();
	}
}


EnemyAttack::EnemyAttack(int attack, int range)
	: EnemyAction(), attack(attack), range(range)
{
	actionDescription = fmt::format("Attack {0} - range {1}", attack, range);
}

bool EnemyAttack::CanPerform(const Actor& actor)
{
	if(actor.Stunned() || actor.Disarmed())
		return false;

	return true;
}

void EnemyAttack::Calculate(Level& level, const Actor& actor)
{
	startPoint = level.TileAt(actor.Position()).WorldPosition();
	auto tiles = level.TilesWithin(actor.Position(), range + actor.Range());

	for (auto tile : tiles) {
		tile->GetHexagon().SetHighlight(glm::vec3(0, 0, 1));
	}

	auto removedTiles = std::remove_if(tiles.begin(), tiles.end(), [&level, &actor, this](const Tile * x) {
		if (!x->IsOccupied())
			return true;

		auto occupant = level.ActorById(x->OccupiedId());
		if (occupant->Team() == actor.Team())
			return true;

		return false; });

	tiles.erase(removedTiles, tiles.end());

	for (auto tile : tiles) {
		tile->GetHexagon().SetHighlight(glm::vec3(1, 0, 0));
	}

	if (tiles.size() > 0) {
		auto tile = tiles[0];
		auto target = std::pair<glm::ivec3, glm::vec3>(tile->Location(), tile->WorldPosition());
		targets.push_back(target);
	}
}

void EnemyAttack::Perform(Level& level, Actor& attacker)
{
	level.ClearHighlights();

	for (auto target : targets) {
		auto& tile = level.TileAt(target.first);
		auto actor = level.ActorById(tile.OccupiedId());

		auto statusEffects = std::vector<StatusEffect>{};
		level.PerformAttack(attack, statusEffects, attacker, level.monsterModifiers, *actor);
	}
}

void EnemyAttack::Render()
{
	for (auto target : targets) {
		glBegin(GL_LINES);
		glVertex2fv(&startPoint.x);
		glVertex2fv(&target.second.x);
		glEnd();
	}
}

EnemyHealSelf::EnemyHealSelf(int healAmount)
	: EnemyAction(), heal(healAmount), calculated(false)
{
	actionDescription = fmt::format("Heal self {0}", healAmount);
}

bool EnemyHealSelf::CanPerform(const Actor& actor)
{
	if( actor.Stunned() )
		return false;
	return true;
}

void EnemyHealSelf::Calculate(Level& level, const Actor& actor)
{
	calculated = true;
	actorWorldPosition = actor.WorldPosition();
}

void EnemyHealSelf::Perform(Level& level, Actor& actor)
{
	auto healedFor = actor.DoHealing(heal);
	level.combatLog.push_back(fmt::format("{0} healed for {1}", actor.Name(), healedFor));
}

#include"..//icons/icons.h"
#include"..//render.h"
void EnemyHealSelf::Render()
{
	if( !calculated )
		return;

	auto texture = Icons::Get("heal");
	Render::Quad(actorWorldPosition, glm::vec2(15,15), glm::vec3(0,1,0));
}


EnemyShieldSelf::EnemyShieldSelf(int shield)
	: EnemyAction(), shield(shield), calculated(false)
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

void EnemyShieldSelf::Perform(Level& level, Actor& actor)
{
	actor.ModifyShield(shield);
	int s = shield;
	actor.AddEndOfRoundAction([s](Actor* a) { a->ModifyShield(-s); });
}

void EnemyShieldSelf::Render()
{
	if( !calculated )
		return;

	auto texture = Icons::Get("shield");
	Render::Quad(actorWorldPosition, glm::vec2(15,15), *texture, glm::vec3(0,1,0), true);
}

EnemyRetaliate::EnemyRetaliate(int retaliate)
	: EnemyAction(), retaliate(retaliate), calculated(false)
{
	actionDescription = fmt::format("Retaliate {0}", retaliate);
}

bool EnemyRetaliate::CanPerform(const Actor& actor)
{
	return !actor.Stunned();
}

void EnemyRetaliate::Calculate(Level& level, const Actor& actor)
{
	calculated=true;
	actorWorldPosition = actor.WorldPosition();
}

void EnemyRetaliate::Perform(Level& level, Actor& actor)
{
	actor.ModifyRetaliate(retaliate);
	int r = retaliate;
	actor.AddEndOfRoundAction([r](Actor* a) { a->ModifyRetaliate(-r); });
}

void EnemyRetaliate::Render()
{
	if( !calculated )
		return;

	auto texture = Icons::Get("retaliate");
	Render::Quad(actorWorldPosition, glm::vec2(15, 15), *texture, glm::vec3(1, 0, 0), true);
}
