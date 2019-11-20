#include "pch/pch.h"
#include "Action.h"

ActionElement::ActionElement(Level& level, Actor& actor)
	: Action(level, actor)
{
}

void ActionElement::Click(Director& director, const glm::ivec3& target)
{
}

void ActionElement::Undo()
{
}

void ActionElement::Reset()
{
}

bool ActionElement::Perform(Director& director, Actor& actor)
{
	return false;
}

void ActionElement::Render()
{
}
