#pragma once

#include<vector>
#include<functional>
#include"Tile.h"
#include"../action/Spawner.h"

class TextService;
class Actor;
class Entity;

class Level
{
public:
	Level();
	~Level();

	void Generate();
	void Spawn();

	void Update(const glm::vec2&);
	void Render(const TextService& text);

	void Highlight(const glm::ivec3& center, int range, const glm::vec3& highlightColor, std::function<bool(const Tile&)> highlightPredicate);
	void Highlight(const glm::ivec3& center, int range, const glm::vec3& highlightColor);
	void ClearHighlights();
	
	std::vector<Tile*> TilesWithin(const glm::ivec3& center, int range);

	bool HasHoverTarget() const { return (hoverTarget != nullptr); }
	Tile& GetHoverTarget() { return *hoverTarget;  }

	Tile& TileAt(const glm::ivec3& location);

	Actor* ActorById(int actorId);
	Actor* GetPlayer();

	void AddEntity(Entity* entity);

	void ShowCoords(bool);

private:
	Spawner spawner;
	bool showCoords;

	glm::ivec3 GetCenterCoord(int x, int y, int z);

	std::vector<Entity*> entities;
	std::vector<Tile*> tiles;

	Tile* hoverTarget;
	Tile* selectedHex;
	glm::ivec3 selectedCoord;

	bool hasDistance;
	glm::ivec3 distance;
};