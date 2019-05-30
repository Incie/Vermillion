#pragma once


#include"..//uilayer.h"
#include<functional>

class EditorTile;

class TileModifier : public UILayer
{
public:
	TileModifier(std::function<void(int)> callback);
	~TileModifier();

	bool HandleInput(const InputService& inputService);
	void Resize(const glm::vec2& windowSize);
	void Measure(const glm::vec2& windowSize);
	void Render(ServiceLocator& Services);

	void OnEvent(WindowEvent type, int id);

	void SetTile(EditorTile* tile);

	UILayerId LayerId() override;

private:
	void SetTileEntity(const std::string& entity);
	void ClearButtonStates();
	UIElement* GetChildById(int id);

	std::function<void(int)> callback;
	EditorTile* tile;
};