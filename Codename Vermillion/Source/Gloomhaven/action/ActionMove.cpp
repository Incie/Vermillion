#include"pch.h"
#include "ActionMove.h"
#include"../entity/Entity.h"
#include"../level/Level.h"


ActionMove::ActionMove(Level&level, Actor& actor, int moveMax, bool flying)
	: Action(level, actor), moveMax(moveMax), movesLeft(moveMax), flying(flying)
{
	actionDescription = fmt::format("Move {0} Flying/Jumping [{1}]", moveMax, flying);
	currentPosition = actor.Position();
	Highlight();
}

ActionMove::~ActionMove()
{
}

void ActionMove::Click(const glm::ivec3& target)
{
	if (movesLeft == 0)
		return;

	auto& clickedTile = level.TileAt(target);

	if (clickedTile.DistanceTo(currentPosition) != 1)
		return;

	if (clickedTile.IsOccupied() && clickedTile.DistanceTo(actor.Position()) != 0 && !flying )
		return;

	currentPosition = clickedTile.Location();
	movesLeft--;

	for(auto containingEntity : clickedTile.ContainingEntities()) {
		if(!containingEntity->Active())
			continue;

		if(containingEntity->Name().rfind("Door", 0) == 0) {
			//open door

			auto surroundingTiles = level.TilesWithin(clickedTile.Location(), 1);

			int currentRoom = clickedTile.RoomNumber();
			int nextRoom = -1;

			for(auto st : surroundingTiles) {
				if(st->RoomNumber() != currentRoom) {
					nextRoom = st->RoomNumber();
				}
			}

			level.SpawnRoom(nextRoom);

			containingEntity->Deactivate();
		}
	}

	//todo: lock in movement

	plannedRoute.push_back(currentPosition);
	Highlight();
}

void ActionMove::Undo()
{
	if( plannedRoute.size() > 0 )
		plannedRoute.pop_back();

	if( plannedRoute.size() > 0 )
	{
		currentPosition = plannedRoute[plannedRoute.size() - 1];
		movesLeft++;
		Highlight();
		return;
	}

	currentPosition = actor.Position();
	movesLeft = moveMax;
	Highlight();
}

void ActionMove::Reset()
{
	plannedRoute.clear();
	level.ClearHighlights();
}

bool ActionMove::Perform(Actor& actor)
{
	auto& tile = level.TileAt(currentPosition);

	if (tile.IsOccupied() && tile.DistanceTo(actor.Position()) != 0 )
		return false;

	
	auto& startTile = level.TileAt(actor.Position());
	startTile.SetOccupied(-1);
	tile.SetOccupied(actor.EntityId());

	actor.SetPosition(tile.Location(), tile.WorldPosition());

	level.ClearHighlights();
	return true;
}

#include"GL/glew.h"
void ActionMove::Render()
{
	if (plannedRoute.size() == 0)
		return;

	glBegin(GL_LINES);
		auto& h0 = level.TileAt(actor.Position());
		auto& h1 = level.TileAt(plannedRoute[0]);
		glVertex2fv(&h0.WorldPosition().x); glVertex2fv(&h1.WorldPosition().x);

		for (int i = 0; i < plannedRoute.size() - 1; ++i) {
			auto& hex0 = level.TileAt(plannedRoute[i]);

			int j = 1 + i;
			auto& hex1 = level.TileAt(plannedRoute[j]);
			glVertex2fv(&hex0.WorldPosition().x); glVertex2fv(&hex1.WorldPosition().x);
		}
	glEnd();
}

void ActionMove::Highlight()
{
	level.ClearHighlights();

	if( movesLeft > 1 )
		level.Highlight(currentPosition, movesLeft, glm::vec3(0, 0, 1), flying);

	if( movesLeft > 0 )
		level.Highlight(currentPosition, 1, glm::vec3(0.5f, 0, 0), flying);

	level.Highlight(currentPosition, 0, glm::vec3(0, 1, 0));
}

ActionTrample::ActionTrample(Level& level, Actor& actor, int moveMax, bool flying, int attack)
	: ActionMove(level, actor, moveMax, flying), attack(attack)
{
}

ActionTrample::~ActionTrample()
{
}

bool ActionTrample::Perform(Actor& actor)
{
	auto& tile = level.TileAt(currentPosition);

	if(tile.IsOccupied() && tile.DistanceTo(actor.Position()) != 0)
		return false;


	for(auto tileLocation : plannedRoute) {
		auto& tile = level.TileAt(tileLocation);
		
		if(!tile.IsOccupied())
			continue;

		//do Attack
		auto targetActor = level.ActorById(tile.OccupiedId());

		int calculatedDamage = attack;
		auto modifiers = level.playerModifiers.Draw();
		
		for(auto m : modifiers)
			calculatedDamage = m.ModifyValue(calculatedDamage);


		int actualDamage = targetActor->DoDamage(calculatedDamage, 0);
		level.combatLog.push_back(fmt::format("{0} did {1} ({4} + {3}) damage to {2}", "[Player]", actualDamage, "[Enemy]", Modifier::ToString(modifiers), attack));


		if(targetActor->Health() <= 0) {
			level.combatLog.push_back("[Enemy] died");
			level.RemoveActorById(targetActor->EntityId());
			targetActor->Deactivate();
		}
	}

	return ActionMove::Perform(actor);
}
