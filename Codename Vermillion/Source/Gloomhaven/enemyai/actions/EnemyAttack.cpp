#include"pch.h"
#include"EnemyAttack.h"
#include"../../entity/Entity.h"
#include"../../level/Level.h"
#include"../../Director.h"
#include"GL/glew.h"


EnemyAttack::EnemyAttack(int attack, int range, int targets, std::vector<StatusEffect> statusEffects)
	: EnemyAction(), attack(attack), originalAttack(attack), range(range), originalRange(range)
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
	auto tiles = level.TilesWithin(actor.Position(), range);

	for(auto tile : tiles) {
		tile->GetHexagon().SetHighlight(glm::vec3(0, 0, 1));
	}

	auto removedTiles = std::remove_if(tiles.begin(), tiles.end(), [&level, &actor, this](const Tile* x) {
		if(!x->IsOccupied())
			return true;

		auto occupant = level.ActorById(x->OccupiedId());
		if(occupant->Team() == actor.Team())
			return true;

		return false; 
	});

	tiles.erase(removedTiles, tiles.end());

	for(auto tile : tiles) {
		tile->GetHexagon().SetHighlight(glm::vec3(1, 0, 0));
	}

	if(tiles.size() > 0) {
		auto tile = tiles[0];
		auto target = std::pair<glm::ivec3, glm::vec3>(tile->Location(), tile->WorldPosition());
		targets.push_back(target);
	}
}

void EnemyAttack::Perform(Director& director, Level& level, Actor& attacker)
{
	level.ClearHighlights();

	for(auto target : targets) {
		auto& tile = level.TileAt(target.first);
		auto actor = level.ActorById(tile.OccupiedId());

		auto statusEffects = std::vector<StatusEffect>{};
		director.PerformAttack(attack, range, statusEffects, attacker, level.monsterModifiers, *actor);
	}
}

void EnemyAttack::Render()
{
	for(auto target : targets) {
		glBegin(GL_LINES);
		glVertex2fv(&startPoint.x);
		glVertex2fv(&target.second.x);
		glEnd();
	}
}

void EnemyAttack::Modify(const Actor& actor)
{
	range = originalRange + actor.Range();
	attack = originalAttack + actor.Attack();

	actionDescription = fmt::format("Attack {0} ({2} + {4})- range {1} ({3} + {5})", attack, range, originalAttack, originalRange, actor.Attack(), actor.Range());
}