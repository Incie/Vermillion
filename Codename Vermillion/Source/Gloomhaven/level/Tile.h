#pragma once

#include"glm/glm.hpp"
#include"Hexagon.h"

class Entity;

class Tile {
public:
	Tile(glm::ivec3 location, glm::vec3 worldpos);
	Tile(const Tile&) = delete;
	~Tile();
	
	Tile& operator=(const Tile&) = delete;

	int OccupiedId() const { return occupyingActorId; };
	bool IsOccupied() const;
	void SetOccupied(int actorId);

	Hexagon& GetHexagon() { return hexagon; }

	void AddEntity(Entity* entity) { containingEntities.push_back(entity); }
	const std::vector<Entity*>& ContainingEntities() const { return containingEntities; }
	bool ContainsEntities() const { return containingEntities.size() != 0; }

	const glm::ivec3& Location() const { return cubeLocation; }
	const glm::vec3& WorldPosition() const { return worldPosition; }

	int DistanceTo(const glm::ivec3& p) const;
	double DistanceFromCenterTo(const glm::vec2& point) const;

	int RoomNumber() const { return roomNumber; }
	void RoomNumber(int roomNumber) { this->roomNumber = roomNumber; }

	bool Enabled() const { return enabled; }
	void Enable() { enabled = true; }
	void Disable() { enabled = false; }
	
protected:
	bool enabled;

	std::vector<Entity*> containingEntities;
	int occupyingActorId;
	Hexagon hexagon;

	int roomNumber;
	glm::ivec3 cubeLocation;
	glm::vec3 worldPosition;
};


class EditorTile : public Tile
{
public:
	EditorTile(glm::ivec3 location, glm::vec3 worldpos);
	~EditorTile();

	//room #

	//terraintype 
	 // normie, difficult, hazard, obstacle, door
	//contains entity
	 // coin/trap/treasure/enemy

	std::string entityName;
	Hexagon* entity;
};