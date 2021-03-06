#include"pch.h"
#include "Tile.h"
#include"..//entity/Entity.h"

Tile::Tile(glm::ivec3 location, glm::vec3 worldpos)
	: occupyingActorId(-1), cubeLocation(location), worldPosition(worldpos), enabled(true), roomNumber(1)
{
}

Tile::~Tile()
{
	for(auto ce : containingEntities)
		delete ce;

	containingEntities.clear();
}

bool Tile::IsOccupied() const
{
	return occupyingActorId >= 0;
}

void Tile::SetOccupied(int actorId)
{
	occupyingActorId = actorId;
}

#include<algorithm>

int Tile::DistanceTo(const glm::ivec3& p) const
{
	auto i = std::max<int>(abs(p.x - cubeLocation.x), abs(p.y - cubeLocation.y));
	return std::max(i, abs(p.z - cubeLocation.z));
}

double Tile::DistanceFromCenterTo(const glm::vec2 & point) const
{
	glm::vec3 v = worldPosition - glm::vec3(point.x, point.y, 0.0f);
	return glm::distance(glm::vec3(point.x, point.y, 0.0f), worldPosition);
}


// === === === === === ===
 
EditorTile::EditorTile(glm::ivec3 location, glm::vec3 worldpos)
	: Tile(location, worldpos), entity(nullptr)
{
}

EditorTile::~EditorTile()
{
}
