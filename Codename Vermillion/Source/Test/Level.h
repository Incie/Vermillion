#pragma once

#include<vector>
#include"../Utils/Hexagon.h"
#include"../Framework/services.h"
#include"../Framework/text.h"

#include"Spawner.h"
#include"Tile.h"

class Level
{
public:
	Level();
	~Level();

	void Generate();
	void Spawn();

	void Update(const glm::vec2&);
	void Render(const TextService& text);

	void Highlight(const glm::ivec3& center, int range, const glm::vec3& color);
	void ClearHighlights();
	

	bool HasHoverTarget() const { return (hoverTarget != nullptr); }
	Tile& GetHoverTarget() { return *hoverTarget;  }

	Tile& TileAt(const glm::ivec3& location);

	void AddEntity(Entity* entity);

	void ShowCoords(bool);

private:
	//void RenderHex(const Tile& hex);
	Spawner spawner;
	bool showCoords;

	glm::ivec3 GetCenterCoord(int x, int y, int z);

	std::vector<Entity*> entities;
	std::vector<Tile*> activeLevel;

	Tile* hoverTarget;
	Tile* selectedHex;
	glm::ivec3 selectedCoord;

	bool hasDistance;
	glm::ivec3 distance;
};