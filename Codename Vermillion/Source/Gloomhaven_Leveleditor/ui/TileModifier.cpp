#include"pch.h"
#include "TileModifier.h"
#include"..//Gloomhaven/level/Tile.h"
#include"..//Gloomhaven/icons/icons.h"

TileModifier::TileModifier(std::function<void(const std::string&, const glm::ivec3&)> callback)
	: callback(callback), tile(nullptr)
{
	SetAnchor(WindowAnchor::TOP | WindowAnchor::BOTTOM | WindowAnchor::RIGHT);
	Deactivate();

	AddChild(vnew Button("None", 18, 1) );
	AddChild(vnew Button("Bandit Guard", 18, 2));
	AddChild(vnew Button("Bandit Guard Elite", 18, 3));
	AddChild(vnew Button("Living Bones", 18, 4));
	AddChild(vnew Button("Living Bones Elite", 18, 5));
	AddChild(vnew Button("Bandit Archer", 18, 6));
	AddChild(vnew Button("Bandit Archer Elite", 18, 7));
	AddChild(vnew Button("Obstacle", 18, 8));
	AddChild(vnew Button("Trap", 18, 9));
	AddChild(vnew Button("Coin", 18, 10));
	AddChild(vnew Button("Treasure", 18, 11));
	AddChild(vnew Button("Door (V)", 18, 12));
	AddChild(vnew Button("Door (H)", 18, 13));
	AddChild(vnew Button("Start", 18, 14));
}

TileModifier::~TileModifier()
{
}

bool TileModifier::HandleInput(const InputService& inputService)
{
	return UILayer::HandleInput(inputService);
}

void TileModifier::Resize(const glm::vec2& windowSize, const TextService& text)
{
	UILayer::Resize(windowSize, text);

	float y = 8.0f;

	for (auto child : children) {
		child->SetPosition(8, y);
		child->SetSize(150.0f - 16.0f, 22.0f);

		y += 22.0f + 8.0f;
	}
}

void TileModifier::Measure(const glm::vec2& windowSize, const TextService& text)
{
	size.x = 150.0f;
	size.y = children.size() * 22.0f + (children.size()+1) * 8;
}

void TileModifier::Render(ServiceLocator& Services)
{
	UILayer::Render(Services);
}

void TileModifier::OnEvent(WindowEvent type, int id)
{
	if (type == WindowEvent::CLICK) {
		auto index = id - 1;
		
		ClearButtonStates();
		
		auto button = dynamic_cast<Button*>(children[index]);
		button->SetColor(glm::vec3(0.5f, 1.0f, 0.5f));

		auto child = dynamic_cast<Button*>(GetChildById(id));
		if(child != nullptr) {
			callback(child->Text(), tile->Location());
		}
	}
}

void TileModifier::SetTile(EditorTile* tile)
{
	this->tile = tile;
	if (tile == nullptr) {
		Deactivate();
	}

	Activate();
	ClearButtonStates();
}

UILayerId TileModifier::LayerId()
{
	return UILayerId();
}

void TileModifier::SetTileEntity(const std::string& entityName)
{
	if (tile->entity != nullptr)
		delete tile->entity;

	tile->entity = vnew Hexagon();
	tile->entity->Generate(tile->WorldPosition(), 40, 45);
	
	tile->entityName = entityName;
	tile->entity->SetTexture(Icons::Get(entityName));
}

void TileModifier::ClearButtonStates()
{
	for (auto child : children) {
		auto button = dynamic_cast<Button*>(child);
		button->SetColor(glm::vec3(0.6f));
	}
}

UIElement* TileModifier::GetChildById(int id)
{
	for (auto child : children)
	{
		if (child->Id() == id)
			return child;
	}

	return nullptr;
}
