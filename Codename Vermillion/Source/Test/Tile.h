#pragma once

#include"../Utils/Hexagon.h"
#include"Entity.h"

class Tile {
public:
	Tile(glm::ivec3 location, glm::vec3 worldpos);
	~Tile();

	bool IsOccupied();
	void SetOccupied(int actorId);

	Hexagon& GetHexagon() { return hexagon; }

	glm::ivec3& Location() { return cubeLocation; } 
	glm::vec3& WorldPosition() { return worldPosition; }

	int DistanceTo(const glm::ivec3& p) const;
	double DistanceFromCenterTo(const glm::vec2& point) const;

private:
	std::vector<Entity*> containingEntities;
	int occupyingActor;
	Hexagon hexagon;

	glm::ivec3 cubeLocation;
	glm::vec3 worldPosition;
};