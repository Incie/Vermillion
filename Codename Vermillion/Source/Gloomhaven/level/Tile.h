#pragma once

#include"glm/glm.hpp"
#include"Hexagon.h"

class Entity;

class Tile {
public:
	Tile(glm::ivec3 location, glm::vec3 worldpos);
	~Tile();

	int OccupiedId() const { return occupyingActorId; };
	bool IsOccupied() const;
	void SetOccupied(int actorId);

	Hexagon& GetHexagon() { return hexagon; }

	const glm::ivec3& Location() const { return cubeLocation; }
	const glm::vec3& WorldPosition() const { return worldPosition; }

	int DistanceTo(const glm::ivec3& p) const;
	double DistanceFromCenterTo(const glm::vec2& point) const;

private:
	std::vector<Entity*> containingEntities;
	int occupyingActorId;
	Hexagon hexagon;

	glm::ivec3 cubeLocation;
	glm::vec3 worldPosition;
};