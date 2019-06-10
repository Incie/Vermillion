#include"pch.h"
#include "HoverCard.h"

#include"..//level/Level.h"
#include"..//entity/Entity.h"
#include"..//level/Tile.h"

HoverCard::HoverCard()
	: tile(nullptr), entity_icon(nullptr)
{
	SetAnchor(WindowAnchor::BOTTOM | WindowAnchor::TOP | WindowAnchor::LEFT);
}

HoverCard::~HoverCard()
{
}

void HoverCard::Resize(const glm::vec2& windowSize, const TextService& text)
{
	UIView::Resize(windowSize, text);

	if(tile == nullptr)
		return;

	position.y = 15.0f;
}

void HoverCard::Measure(const glm::vec2& windowSize, const TextService& text)
{
	if(tile == nullptr) {
		size.x = 5;
		size.y = 5;
		return;
	}

	float longestString = 0.0f;
	for(auto& t : texts) {
		longestString = std::max<float>(text.CalculateWidth(t, 16), longestString);
	}

	size.x = longestString + 16.0f;
	size.y = texts.size() * 16.0f + texts.size() * 8.0f + 8.0f;
}

#include"GL/glew.h"
#include"..//servicelocator.h"
#include"..//constants.h"
void HoverCard::Render(ServiceLocator& Services)
{
	UIView::Render(Services);

	glPushMatrix();

	auto& textService = Services.Text();
	for(auto& t : texts) {
		textService.Print(8, 0, t, 16, Colors::Black, false, true);
	}
	glPopMatrix();
}



void HoverCard::SetHoverTile(const Level& level, const Tile* tile)
{

	
	texts.clear();

	if(tile == nullptr) {
		this->tile = nullptr;
		Deactivate();
		Invalidate();
		return;
	}


	if(this->tile == nullptr || tile->Location() == this->tile->Location()) 
		Invalidate();

	this->tile = tile;


	if(!tile->IsOccupied() && !tile->ContainsEntities())
	{
		this->tile = nullptr;
		Deactivate();
		Invalidate();
		return;
	}

	Activate();

	if(tile->IsOccupied()) {
		auto actorId = tile->OccupiedId();

		auto actorView = level.ActorViewById(actorId);
		
		auto playerView = dynamic_cast<const Player*>(actorView);
		if(playerView != nullptr) {
			playerView->PrintStats(texts);
		}

		auto enemyView = dynamic_cast<const Enemy*>(actorView);
		if(enemyView != nullptr) {
			enemyView->PrintStats(texts);
		}
	}
	

	//evaluate whether to just use entity-icons
	if(tile->ContainsEntities()) {

		texts.emplace_back("[Entities]");

		auto containingEntities = tile->ContainingEntities();
		for(auto entity : containingEntities) {
			texts.emplace_back(entity->Name());
		}
	}
}
