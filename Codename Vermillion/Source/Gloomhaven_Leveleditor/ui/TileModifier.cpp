#include"pch.h"
#include "TileModifier.h"
#include"..//Gloomhaven/level/Tile.h"

TileModifier::TileModifier(std::function<void(int)> callback)
	: callback(callback), tile(nullptr)
{
	SetAnchor(WindowAnchor::TOP | WindowAnchor::BOTTOM | WindowAnchor::RIGHT);
	Deactivate();

	auto button = vnew Button();
	button->SetText("None");
	button->SetId(1);
	children.push_back(button);

	button = vnew Button();
	button->SetText("Bandit Guard");
	button->SetId(2);
	children.push_back(button);

	button = vnew Button();
	button->SetText("Living Bones");
	button->SetId(3);
	children.push_back(button);

	button = vnew Button();
	button->SetText("Bandit Archer");
	button->SetId(4);
	children.push_back(button);

	button = vnew Button();
	button->SetText("2");
	button->SetId(5);
	children.push_back(button);

	button = vnew Button();
	button->SetText("3");
	button->SetId(6);
	children.push_back(button);

	button = vnew Button();
	button->SetText("4");
	button->SetId(7);
	children.push_back(button);


	button = vnew Button();
	button->SetText("Obstacle");
	button->SetId(8);
	children.push_back(button);

	button = vnew Button();
	button->SetText("Trap");
	button->SetId(9);
	children.push_back(button);

	button = vnew Button();
	button->SetText("Coin");
	button->SetId(10);
	children.push_back(button);

	button = vnew Button();
	button->SetText("Treasure");
	button->SetId(11);
	children.push_back(button);

	button = vnew Button();
	button->SetText("Door (V)");
	button->SetId(12);
	children.push_back(button);

	button = vnew Button();
	button->SetText("Door (H)");
	button->SetId(13);
	children.push_back(button);
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

#include"..//Gloomhaven/icons/icons.h"

void TileModifier::OnEvent(WindowEvent type, int id)
{
	if (type == WindowEvent::CLICK) {
		auto index = id - 1;
		
		ClearButtonStates();
		
		auto button = dynamic_cast<Button*>(children[index]);
		button->SetColor(glm::vec3(0.5f, 1.0f, 0.5f));

		if (id == 1) {
			if (tile->entity != nullptr)
				delete tile->entity;
			tile->entity = nullptr;
		}
		if(id == 2) SetTileEntity("Bandit Guard");
		if(id == 3) SetTileEntity("Living Bones");
		if(id == 4) SetTileEntity("Bandit Archer");
		if(id == 8) SetTileEntity("Obstacle");
		if(id == 9) SetTileEntity("Trap");
		if(id ==10) SetTileEntity("Coin");
		if(id ==11) SetTileEntity("Treasure");
		if(id == 12) SetTileEntity("Door(V)");
		if(id == 13) SetTileEntity("Door(H)");
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
	//set buttonstates based on tiledata
	auto color = glm::vec3(0.5f, 1.0f, 0.5f);
	if(tile->entityName == "Bandit Guard") dynamic_cast<Button*>(GetChildById(2))->SetColor(color);
	if(tile->entityName == "Living Bones") dynamic_cast<Button*>(GetChildById(3))->SetColor(color);
	if(tile->entityName == "Bandir Archer") dynamic_cast<Button*>(GetChildById(4))->SetColor(color);
	if(tile->entityName == "Obstacle") dynamic_cast<Button*>(GetChildById(8))->SetColor(color);
	if(tile->entityName == "Trap") dynamic_cast<Button*>(GetChildById(9))->SetColor(color);
	if(tile->entityName == "Coin") dynamic_cast<Button*>(GetChildById(10))->SetColor(color);
	if(tile->entityName == "Treasure") dynamic_cast<Button*>(GetChildById(11))->SetColor(color);
	if(tile->entityName == "Door(V)") dynamic_cast<Button*>(GetChildById(11))->SetColor(color);
	if(tile->entityName == "Door(H)") dynamic_cast<Button*>(GetChildById(11))->SetColor(color);
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
