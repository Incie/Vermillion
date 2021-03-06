#pragma once


#include"..//uiview.h"
#include<functional>

class EditorTile;

class TileModifier : public UIView
{
public:
	TileModifier(std::function<void(const std::string&, const glm::ivec3&)> callback);
	~TileModifier();

	bool HandleInput(const InputService& inputService);
	void Resize(const glm::vec2& windowSize, const TextService& text);
	void Measure(const glm::vec2& windowSize, const TextService& text);
	void Render(ServiceLocator& Services);

	void OnEvent(WindowEvent type, int id);

	void SetTile(EditorTile* tile);

private:
	void SetTileEntity(const std::string& entity);
	void ClearButtonStates();
	UIElement* GetChildById(int id);

	std::function<void(const std::string&, const glm::ivec3&)> callback;
	EditorTile* tile;
};