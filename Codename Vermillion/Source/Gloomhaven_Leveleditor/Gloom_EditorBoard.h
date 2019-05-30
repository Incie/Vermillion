#pragma once



#include<vector>
#include"..//Gloomhaven/level/Tile.h"

class InputService;
class TextService;
class UILayer;

class EditorBoard
{
public:
	EditorBoard();
	~EditorBoard();

	void Delete();

	void SetCallback(std::function<void(EditorTile*)> callback);
	void SetMode(int mode);

	void Generate(int x, int y);

	void Clear();
	void Update(const InputService& inputService, glm::vec2& viewCoords);
	void Render(const TextService& textService);


	void SaveToDisk();
	void LoadFromDisk();
private:
	std::function<void(EditorTile*)> callback;
	int mode;

	std::vector<EditorTile*> tiles;
	EditorTile* hover;
	EditorTile* selected;
	float hexagonSize;
};

