#include "ActionMove.h"


ActionMove::ActionMove(Level&level, glm::ivec3 center, int moveMax) 
	: level(level), start(center), moveMax(moveMax), movesLeft(moveMax), currentPosition(center)
{
	Highlight();
}

ActionMove::~ActionMove()
{
}

void ActionMove::Click(const glm::ivec3& target)
{
	if (movesLeft == 0)
		return;

	auto clickedHex = level.TileAt(target);

	if (clickedHex.DistanceTo(currentPosition) != 1)
		return;

	currentPosition = clickedHex.Location();
	movesLeft--;

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

	currentPosition = start;
	movesLeft = moveMax;
	Highlight();
}

void ActionMove::Reset()
{
	plannedRoute.clear();
	level.ClearHighlights();
}

void ActionMove::Perform(Actor& actor)
{
	auto& tile = level.TileAt(currentPosition);
	actor.SetPosition(tile.Location(), tile.WorldPosition());

	level.ClearHighlights();
}

#include"GL/glew.h"
void ActionMove::Render()
{
	if (plannedRoute.size() == 0)
		return;

	glBegin(GL_LINES);
	
		auto& h0 = level.TileAt(start);
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
		level.Highlight(currentPosition, movesLeft, glm::vec3(0, 0, 1));

	if( movesLeft > 0 )
		level.Highlight(currentPosition, 1, glm::vec3(0.5f, 0, 0));

	level.Highlight(currentPosition, 0, glm::vec3(0, 1, 0));
}
