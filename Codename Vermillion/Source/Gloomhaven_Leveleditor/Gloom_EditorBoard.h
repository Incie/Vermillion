#pragma once



#include<vector>
#include"..//Gloomhaven/level/Tile.h"

class InputService;
class TextService;
class UIView;

class EditorBoard
{
public:
	EditorBoard();
	~EditorBoard();

	void Delete();

	void OnTileClick(std::function<void(EditorTile*)> callback);
	
	void SetRoomNumber(int number) { roomNumber = number; }
	void SetMode(int mode);

	void Generate(int x, int y);

	void Clear();
	void Update(const InputService& inputService, glm::vec2& viewCoords);
	void Render(const TextService& textService);

	void SpawnEntity(const std::string& entityName, const glm::ivec3& tile);

	void SaveToDisk();
	void LoadFromDisk();
private:
	EditorTile* GetTileByLocation(const glm::ivec3& location);

	std::function<void(EditorTile*)> callback;
	int mode;
	
	//paint properties
	int roomNumber;

	std::vector<EditorTile*> tiles;
	EditorTile* hover;
	EditorTile* selected;
	float hexagonSize;
};

