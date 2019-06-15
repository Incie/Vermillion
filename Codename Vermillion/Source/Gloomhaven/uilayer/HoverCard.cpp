#include"pch.h"
#include "HoverCard.h"

#include"..//level/Level.h"
#include"..//entity/Entity.h"
#include"..//level/Tile.h"
#include"..//icons/icons.h"

HoverCard::HoverCard()
	: tile(nullptr), entity_icon(nullptr), enemyRound(false), enemyround_texture(nullptr)
{
	enemyround_texture = Icons::Get("monsterabilitycardback");
}

HoverCard::~HoverCard()
{
}

void HoverCard::Resize(const glm::vec2& windowSize, const TextService& text)
{
	UIView::Resize(windowSize, text);

	if(tile == nullptr)
		return;
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

	if(enemyRound) {
		size.x = std::max<float>(316.0f, size.x);
		float scale = 300.0f / static_cast<float>(enemyround_texture->width);
		size.y += scale * enemyround_texture->height;
	}
}


#include"GL/glew.h"
#include"..//servicelocator.h"
#include"..//constants.h"
#include"../render.h"

void HoverCard::Update(float deltaTime, ServiceLocator& services)
{
	const auto& input = services.Input();
	auto mousePosition = input.GetMousePosition();
	SetPosition(mousePosition + glm::vec2(16, 16));
}

void HoverCard::Render(ServiceLocator& Services)
{
	if(Invalidated())
		return;

	UIView::Render(Services);

	glPushMatrix();

	auto& textService = Services.Text();
	for(auto& t : texts) {
		textService.Print(8, 0, t, 16, Colors::Black, false, true);
	}


	if(enemyRound) {
		float desiredWidth = 300.0f;
		float scale = desiredWidth / static_cast<float>(enemyround_texture->width);
		Render::Quad(*enemyround_texture, static_cast<float>(enemyround_texture->height) * scale);
		Services.Text().Print(30, 50, fmt::format("{0}", initiative), static_cast<unsigned int>(20), Colors::White, true);
		float textX = 8 + desiredWidth * 0.5f;
		float textY = 8 + 40.0f;
		float textSize = 18.0f;
		for(auto i = 0; i < enemyRoundTexts.size(); ++i) {
			const auto& action = enemyRoundTexts[i];
			Services.Text().Print(textX, textY, action, static_cast<unsigned int>(textSize), Colors::White, true);
			textY += textSize + 2.0f;
		}
	}

	glPopMatrix();
}

void HoverCard::NoEnemyRound()
{
	enemyRound = false;
	enemyRoundTexts.clear();
}

void HoverCard::SetEnemyRound(const std::vector<std::string>& enemyRound, int initiative)
{
	this->initiative = initiative;
	this->enemyRound = true;
	this->enemyRoundTexts = enemyRound;
}

void HoverCard::SetHoverTile(const Level& level, const Tile* tile)
{	
	if(tile == nullptr) {
		if(this->tile == nullptr)
			return;

		this->tile = nullptr;
		texts.clear();
		Deactivate();
		Invalidate();
		return;
	}

	if(this->tile != nullptr && tile->Location() == this->tile->Location()) 
		return;

	texts.clear();
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
