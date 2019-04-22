#include"pch.h"
#include"EnemyAction.h"
#include"../entity/Entity.h"
#include"../level/Level.h"
#include"GL/glew.h"

EnemyAction::EnemyAction(Level& level)
	: level(level), state(0)
{
}

EnemyAction::~EnemyAction()
{
	targets.clear();
}

EnemyMove::EnemyMove(Level& level, int move)
	: EnemyAction(level), move(move)
{
	actionDescription = fmt::format("Move {0}", move);
}

void EnemyMove::Calculate(const Actor& actor)
{
	startPoint = level.TileAt(actor.Position()).WorldPosition();
	state = 1;

	const auto player = level.GetPlayer();
	const auto playerTile = level.TileAt(player->Position());

	const auto enemyTile = level.TileAt(actor.Position());

	if (playerTile.DistanceTo(enemyTile.Location()) == 1)
		return;

	auto tilesWithinRange = level.TilesWithin(actor.Position(), move);

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

		targets.push_back(tile->Location());
		break;
	}
}

void EnemyMove::Perform(Actor& actor)
{
	level.ClearHighlights();
	auto currentPosition = actor.Position();

	auto moveTargets = Targets();

	if (moveTargets.size() == 1) {
		auto nextPosition = moveTargets[0];
		auto& targetTile = level.TileAt(nextPosition);
		auto& startTile = level.TileAt(currentPosition);

		startTile.SetOccupied(-1);
		targetTile.SetOccupied(actor.EntityId());
		actor.SetPosition(targetTile.Location(), targetTile.WorldPosition());
	}
}

std::vector<glm::ivec3> EnemyAction::Targets()
{
	return targets;
}

void EnemyMove::Render()
{
	for (auto target : targets) {
		glBegin(GL_LINES);
		glVertex2fv(&startPoint.x);
		glVertex2fv(&level.TileAt(target).WorldPosition().x);
		glEnd();
	}
}


EnemyAttack::EnemyAttack(Level& level, int attack, int range)
	: EnemyAction(level), attack(attack), range(range)
{
	actionDescription = fmt::format("Attack {0} - range {1}", attack, range);
}

void EnemyAttack::Calculate(const Actor& actor)
{
	startPoint = level.TileAt(actor.Position()).WorldPosition();
	auto tiles = level.TilesWithin(actor.Position(), range);

	for (auto tile : tiles) {
		tile->GetHexagon().SetHighlight(glm::vec3(0, 0, 1));
	}

	auto removedTiles = std::remove_if(tiles.begin(), tiles.end(), [&actor, this](const Tile * x) {
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
		targets.push_back(tiles[0]->Location());
	}
}

void EnemyAttack::Perform(Actor& attacker)
{
	level.ClearHighlights();

	for (auto target : targets) {
		auto tile = level.TileAt(target);
		auto actor = level.ActorById(tile.OccupiedId());

		int modifier = level.monsterModifiers.Draw();
		int calculatedDamage = attack;
	
		if (modifier == 10)
			calculatedDamage *= 2;
		else if (modifier == -10)
			calculatedDamage = 0;
		else calculatedDamage += modifier;


		int actualDamage = actor->DoDamage(calculatedDamage);
		level.combatLog.push_back(fmt::format("{0} did {1} ({4} + {3}) damage to {2}", "[Enemy]", actualDamage, "[Player]", modifier, attack));
	}
}

void EnemyAttack::Render()
{
	for (auto target : targets) {
		glBegin(GL_LINES);
		glVertex2fv(&startPoint.x);
		glVertex2fv(&level.TileAt(target).WorldPosition().x);
		glEnd();
	}
}
