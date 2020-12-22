#pragma once

#include<memory>
#include"glm/vec2.hpp"
#include"glm/vec3.hpp"
#include<vector>

struct EditorEntity {

};

struct EditorTile {
	glm::vec2 from;
	glm::vec2 to;
	glm::ivec2 gridCoord;

	glm::vec2 verts[4];
	glm::vec3 colors[4];

	//texture
	//texturecoords
	//states
};

struct EditorPage {
	glm::ivec2 from;
	glm::ivec2 to;

	std::vector<EditorTile> background;
	std::vector<EditorTile> foreground;
	std::vector<EditorEntity> entities;
};

class EditorLevel
{
public:
	void AddPage();
	void RemovePage(const glm::ivec2& atWorldCoord);

	void AddTileAt(const glm::ivec2& gridCoord);
	void RemoveTileAt(const glm::ivec2& gridCoord);

	int TileCount();

	void SaveLevel();
	void LoadLevel();

	void Render();
private:
	std::shared_ptr<EditorPage> GetPage(const glm::ivec2& gridCoord);
	bool PageExists(const glm::ivec2& gridCoord);
	bool TileExists(const glm::ivec2& gridCoord);
	
	void CalculatePageSize(std::shared_ptr<EditorPage>& page);
	EditorTile CreateTile(const glm::ivec2& gridCoord);

	std::vector<std::shared_ptr<EditorPage>> pages;
};