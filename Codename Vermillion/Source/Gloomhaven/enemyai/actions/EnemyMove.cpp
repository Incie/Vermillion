#include"pch.h"
#include"EnemyMove.h"
#include"..//..//entity/Entity.h"
#include"..//..//level/Level.h"
#include"GL/glew.h"

EnemyMove::EnemyMove(int move)
	: EnemyAction(), move(move), originalMove(move)
{
	actionDescription = fmt::format("Move {0}", move);
}


bool EnemyMove::CanPerform(const Actor& actor)
{
	if(actor.Immobilised() || actor.Stunned())
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

	if(playerTile.DistanceTo(enemyTile.Location()) == 1)
		return;

	auto tilesWithinRange = level.TilesWithin(actor.Position(), move);

	if(tilesWithinRange.size() == 0)
		return;

	int closest = 99;
	for(auto tile : tilesWithinRange) {
		int distanceToTarget = tile->DistanceTo(playerTile.Location());
		if(distanceToTarget < closest)
			closest = distanceToTarget;
	}

	if(closest == 99)
		return;

	std::sort(tilesWithinRange.begin(), tilesWithinRange.end(), [player](auto const x, auto const y) {
		auto d0 = x->DistanceTo(player->Position());
		auto d1 = y->DistanceTo(player->Position());
		return d0 < d1;
		});

	//also consider least movement

	for(auto tile : tilesWithinRange) {
		if(tile->IsOccupied())
			continue;

		targets.push_back(std::pair<glm::ivec3, glm::vec3>(tile->Location(), tile->WorldPosition()));
		break;
	}
}

void EnemyMove::Perform(Director& director, Level& level, Actor& actor)
{
	level.ClearHighlights();
	auto currentPosition = actor.Position();

	auto moveTargets = Targets();

	if(moveTargets.size() == 1) {
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
	for(auto target : targets) {
		glBegin(GL_LINES);
		glVertex2fv(&startPoint.x);
		glVertex2fv(&target.second.x);
		glEnd();
	}
}

void EnemyMove::Modify(const Actor& actor)
{
	move = originalMove + actor.Move();
	actionDescription = fmt::format("Move {0} ({2}+{1})", move, originalMove, actor.Move());
}