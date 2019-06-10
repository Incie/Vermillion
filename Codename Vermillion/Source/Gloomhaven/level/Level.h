#pragma once

#include<vector>
#include<functional>
#include"Tile.h"
#include"../level/Spawner.h"
#include"../cards/Modifiers.h"

class TextService;
class Actor;
class Entity;

class Level
{
public:
	Level();
	~Level();

	void LoadMap(const std::string& fileName);
	void Generate();
	void SpawnRoom(int roomNumber);

	void Update(const glm::vec2&);
	void Render(const TextService& text);

	void Highlight(const glm::ivec3& center, int range, const glm::vec3& highlightColor, std::function<bool(const Tile&)> highlightPredicate);
	void Highlight(const glm::ivec3& center, int range, const glm::vec3& highlightColor, bool ignoreOccupied=false);
	void ClearHighlights();
	
	std::vector<Tile*> TilesWithin(const glm::ivec3& center, int range);

	bool HasHoverTarget() const { return (hoverTarget != nullptr); }
	Tile& GetHoverTarget() { return *hoverTarget;  }

	Tile& TileOccupiedBy(int entityId);
	Tile& TileAt(const glm::ivec3& location);

	void RemoveActorById(int actorId);

	const Actor* ActorViewById(int actorId) const;
	Actor* ActorById(int actorId);
	Actor* GetPlayer();
	std::vector<Actor*> Monsters();

	void AddEntity(Entity* entity);

	void ShowCoords(bool);

	ModifierDeck monsterModifiers;
	ModifierDeck playerModifiers;

	std::vector<std::string> combatLog;

	glm::vec2 Center();

private:
	Spawner spawner;
	bool showCoords;

	glm::ivec3 GetCenterCoord(int x, int y, int z);

	std::vector<Entity*> entities;
	std::vector<Tile*> tiles;

	Tile* hoverTarget;
	Tile* selectedHex;
	glm::ivec3 selectedCoord;

	float hexagonSize;

	bool hasDistance;
	glm::ivec3 distance;
};