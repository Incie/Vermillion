#include"pch.h"
#include "Gloom.h"
#include"GL/glew.h"
#include"glm/glm.hpp"
#include<vector>
#include<Windows.h>
#include"level/Hexagon.h"
#include"level/Level.h"
#include"../windowstate.h"
#include"../log.h"
#include"fmt/format.h"


#include"entity/Entity.h"

#include"action/ActionAttack.h"
#include"action/ActionMove.h"

#include"enemyai/EnemyAI.h"
#include"enemyai/EnemyAction.h"
#include"enemyai/EnemyRound.h"

#include"icons/icons.h"

#include"../uilayer.h"
#include"uilayer/AbilitySelector.h"
#include"uilayer/CardSelection.h"
#include"uilayer/CardSelector.h"
#include"uilayer/InitiativeUI.h"
#include"uilayer/PortraitPanel.h"
#include"uilayer/StatusBar.h"
#include"uilayer/EnemyAdvancer.h"
#include"uilayer/StatusBar.h"


Gloom::Gloom() 
	: director(level, [this](auto eventId) {this->OnDirectorEvent(eventId); }), cardGenerator(level)
{
}

Gloom::~Gloom()
{
}

void Gloom::Initialize()
{
	cards = cardGenerator.PlayerCards();
	InitializeUI();

	level.Generate();
	level.Spawn();

	Icons::Load(Services().Textures());
}

void Gloom::Deinitialize()
{
	for (auto layer : layers)
		delete layer;
	layers.clear();

	Icons::Unload();
}

void Gloom::Resize()
{
	Log::Info("CardRendering", "Resize");
	const auto& windowSize = WindowState::Size();
	glm::vec2 newWindowSize{ windowSize.x, windowSize.y };
	for (auto layer : layers)
		layer->Resize(newWindowSize);
}

void Gloom::Update(double deltaTime)
{
	const auto& input = Services().Input();

	if (input.KeyOnce(VK_F3))
		level.ShowCoords(true);

	if (input.KeyOnce('B'))
		camera.SetPositionTopLeft(glm::vec2());

	if (input.KeyDown(VK_RBUTTON)) {
		const auto& deltaMouse = input.GetMouseDelta();
		camera.Move(deltaMouse);
	}

	if (input.KeyDown(VK_ADD)) camera.ZoomByFactor(0.9f);
	if (input.KeyDown(VK_SUBTRACT)) camera.ZoomByFactor(1.1f);

	bool inputHandled = false;
	for (auto layer : layers) {
		if (layer->Active() == false)
			continue;

		if( !inputHandled )
			inputHandled = layer->HandleInput(input);
	}

	glm::vec2 cameraMouse = camera.ScreenToViewCoords(input.GetMousePositionNormalized());
	level.Update(cameraMouse);
	director.Update(input);
}

void Gloom::Render()
{
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);

	float ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	float diffuseLight[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	float specularLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	glm::vec4 lightDirection = glm::vec4(0.355336f, 0.906561, -0.227779, 0.0);

	glLightfv(GL_LIGHT0, GL_POSITION, &lightDirection.x);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);

	const auto& text = Services().Text();

	camera.Push();
		glPushMatrix();
			glBegin(GL_LINES);
				glVertex2f(0, -500); glVertex2f(0, 500);
				glVertex2f(-500, 0); glVertex2f(500, 0);
			glEnd();
		glPopMatrix();

		level.Render(text);

		director.Render();
	camera.Pop();

	glDisable(GL_LIGHTING);

	director.RenderUI(text);

	for (auto layer : layers) {
		if (layer->Active() == false)
			continue;
		layer->StartRender();
		layer->Render(Services());
		layer->EndRender();
	}

	if (level.combatLog.size() > 0) {
		glPushMatrix();
		glTranslatef(0, 500.0f, 0);
		for (const auto& line : level.combatLog) {
			text.Print(0, 0, line, 20, Colorf(1), false, true);
		}
		glPopMatrix();
	}

	if (level.HasHoverTarget()) {
		const auto& hoverTile = level.GetHoverTarget();
		
		if (hoverTile.IsOccupied()) {
			const auto hoverActor = level.ActorById(hoverTile.OccupiedId());
			glPushMatrix();
			glTranslatef(5, 50, 0);
			hoverActor->PrintStats(text);
			glPopMatrix();
		}
	}
}

void Gloom::InitializeUI()
{
	auto cardSelector = new CardSelect(cardGenerator.PlayerCards(), *Icons::GetPlayerCard(), [this](const std::string & cardName) {
		auto cardFound = std::find_if(cards.begin(), cards.end(), [&cardName](auto playerCard) { if (playerCard.Name().compare(cardName) == 0) return true; return false; });
		if (cardFound == cards.end())
			throw "card not found";

		auto cardSelection = dynamic_cast<CardSelection*>(layers[1]);
		if (cardSelection == nullptr)
			throw "layer not found";

		cardSelection->AddCard(*cardFound);
		});
	cardSelector->SetSize(0, 150.0f);
	cardSelector->SetAnchor(UILayer::WindowAnchor::BOTTOM | UILayer::WindowAnchor::LEFT | UILayer::WindowAnchor::RIGHT);
	cardSelector->Activate();
	layers.push_back(cardSelector);

	auto cardSelection = new CardSelection(*Icons::GetPlayerCard(), [this](CardSelection & cs, int eventId) {
		auto cardName0 = cs.Card(0);
		auto cardName1 = cs.Card(1);

		cs.Deactivate();

		auto cardSelector = dynamic_cast<CardSelect*>(layers[0]);
		cardSelector->Deactivate();

		auto abilitySelector = dynamic_cast<AbilitySelector*>(layers[2]);
		auto playerCard0 = std::find_if(cards.begin(), cards.end(), [&cardName0](auto playerCard) { if (playerCard.Name().compare(cardName0) == 0) return true; return false; });
		auto playerCard1 = std::find_if(cards.begin(), cards.end(), [&cardName1](auto playerCard) { if (playerCard.Name().compare(cardName1) == 0) return true; return false; });

		abilitySelector->SetCards(&(*playerCard0), &(*playerCard1));
		abilitySelector->Activate();


		auto initiativeCard = *playerCard0;
		level.GetPlayer()->Initiative(initiativeCard.Initiative());

		director.StartRound();
	});
	cardSelection->SetSize(0, 0);
	cardSelection->SetAnchor(UILayer::WindowAnchor::RIGHT | UILayer::WindowAnchor::TOP | UILayer::WindowAnchor::BOTTOM);
	cardSelection->Activate();
	layers.push_back(cardSelection);


	auto abilitySelector = new AbilitySelector(*Icons::GetPlayerCard(), [this](auto eventId, auto abilityEnum) {
		if (eventId == 0) {
			director.EndPlayerTurn();
		}
		else {
			if (eventId == 1) {
				switch (abilityEnum) {
				case AbilitySelector::ABILITY_DEFAULT_TOP:
					director.SetPlayerRound( cardGenerator.GetDefaultTop( *level.GetPlayer() ) );
					break;
				case AbilitySelector::ABILITY_DEFAULT_BOTTOM:
					director.SetPlayerRound(cardGenerator.GetDefaultBottom(*level.GetPlayer()));
					break;
				case AbilitySelector::ABILITY_TOP:
				case AbilitySelector::ABILITY_BOTTOM:
				default: 
					director.SetPlayerRound(); 
					break;
				}
			}
			else if (eventId == 2) {
				switch (abilityEnum) {
				case AbilitySelector::ABILITY_DEFAULT_TOP:
					director.SetPlayerRound(cardGenerator.GetDefaultTop(*level.GetPlayer()));
					break;
				case AbilitySelector::ABILITY_DEFAULT_BOTTOM:
					director.SetPlayerRound(cardGenerator.GetDefaultBottom(*level.GetPlayer()));
					break;
				case AbilitySelector::ABILITY_TOP:
				case AbilitySelector::ABILITY_BOTTOM:
				default: 
					director.SetPlayerRound(); 
					break;
				}
			}			
		}
	});
	abilitySelector->SetCards(&cards[0], &cards[1]);
	abilitySelector->Deactivate();
	layers.push_back(abilitySelector);

	auto enemyAdvancer = new EnemyAdvancer([this]() {
		director.AdvanceEnemy();
	});
	layers.push_back(enemyAdvancer);

	auto statusBar = new StatusBar();
	layers.push_back(statusBar);

	auto initiativeTrackerUI = new InitiativeTrackerUI(director.GetInitiativeTracker());
	layers.push_back(initiativeTrackerUI);
}

void Gloom::OnDirectorEvent(DirectorEvent eventId)
{
	switch (eventId) {
		case DirectorEvent::EndOfRound: {
			layers[0]->Activate();
			layers[1]->Activate();
		
			auto cardSelection = dynamic_cast<CardSelection*>(layers[1]);
			cardSelection->ClearCards();

			layers[2]->Deactivate();
			layers[3]->Deactivate();

			auto statusbar = dynamic_cast<StatusBar*>(layers[4]);
			statusbar->SetStatusText("End of Round");
			statusbar->NextRound();

			level.GetPlayer()->EndOfRoundActions();
			break;
		}
		case DirectorEvent::EnemyTurn: {
			layers[2]->Deactivate();
			layers[3]->Activate();

			auto statusbar = dynamic_cast<StatusBar*>(layers[4]);
			statusbar->SetStatusText("AI Turn");
			break;
		}
		case DirectorEvent::PlayerTurn:{
			layers[2]->Activate();
			layers[3]->Deactivate();

			auto statusbar = dynamic_cast<StatusBar*>(layers[4]);
			statusbar->SetStatusText("Player Turn");
			break;
		}
	}
}
