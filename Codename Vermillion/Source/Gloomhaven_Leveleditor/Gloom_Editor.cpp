#include"pch.h"
#include<Windows.h>
#include "Gloom_Editor.h"
#include"..//Gloomhaven/level/Hexagon.h"
#include"..//camera2d.h"
#include"..//Gloomhaven/level/Tile.h"

GloomEditor::GloomEditor()
{
}

GloomEditor::~GloomEditor()
{
}

#include"..//Gloomhaven/icons/icons.h"
#include"ui/TileModifier.h"
#include"ui/SaveLoadUI.h"
#include"ui/PaintProperties.h"

void GloomEditor::Initialize()
{
	Icons::Load(Services().Textures());

	layers.push_back(vnew ActionSelector([&](auto i) {
		if(i == 1) { //paint mode
			editorBoard.SetMode(0);

			layers[1]->Deactivate();
			layers[3]->Activate();
		}
		else if (i == 2) { //select mode
			editorBoard.SetMode(1);

			layers[3]->Deactivate();
		}
	}));
	layers.push_back(vnew TileModifier([&](const std::string& entityName, const glm::ivec3& location) {
		editorBoard.SpawnEntity(entityName, location);
	}));
	layers.push_back(vnew EditorMainMenu([&](auto i) {
		if(i == 0)
			editorBoard.LoadFromDisk(); //load

		if(i == 1) //save
			editorBoard.SaveToDisk();
	}));
	layers.push_back(vnew PaintProperties([&](auto i) {
		editorBoard.SetRoomNumber(i);
	}));

	editorBoard.OnTileClick([&](EditorTile* tile) {
		dynamic_cast<TileModifier*>(layers[1])->SetTile(tile);
	});
	editorBoard.Generate(25,25);
}

void GloomEditor::Deinitialize()
{
	Icons::Unload();

	for (auto layer : layers)
		delete layer;
	layers.clear();

}

#include"..//windowstate.h"
void GloomEditor::Resize(const glm::ivec2& newWindowSize)
{
	auto windowSize = glm::vec2(static_cast<float>(newWindowSize.x), static_cast<float>(newWindowSize.y));
	for (auto layer : layers) {
		layer->Resize(windowSize, Services().Text());
	}
}

void GloomEditor::Update(float delta)
{
	auto& input = Services().Input();

	if (input.KeyDown(VK_RBUTTON)) {
		const auto& deltaMouse = input.GetMouseDelta();
		camera.Move(deltaMouse);
	}

	bool inputHandled = false;
	auto mouseCoords = input.GetMousePosition();
	for (auto layer : layers) {
		if(!layer->Active() || inputHandled )
			continue;

		if(layer->Invalidated()) {
			Resize(WindowState::Size());
		}

		if (layer->HandleInput(input)) {
			inputHandled = true;
		}
	}

	auto normalizedMouseCoords = input.GetMousePositionNormalized();
	auto viewCoords = camera.ScreenToViewCoords(normalizedMouseCoords);
	
	if (inputHandled)
		editorBoard.Clear();
	else editorBoard.Update(input, viewCoords);
}

#include"GL/glew.h"
void GloomEditor::Render()
{
	auto& text = Services().Text();
	camera.Push();
		editorBoard.Render(text);
	camera.Pop();
	
	for (auto layer : layers) {

		if(!layer->Active())
			continue;

		layer->StartRender();
		layer->Render( Services() );
		layer->EndRender();
	}

	glDisable(GL_TEXTURE_2D);
}
