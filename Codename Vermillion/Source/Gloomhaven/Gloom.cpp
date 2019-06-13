#include"pch.h"
#include"Gloom.h"
#include"..//render.h"

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

#include"../uiview.h"
#include"uilayer/AbilitySelector.h"
#include"uilayer/CardSelection.h"
#include"uilayer/CardSelector.h"
#include"uilayer/InitiativeUI.h"
#include"uilayer/PortraitPanel.h"
#include"uilayer/StatusBar.h"
#include"uilayer/EnemyAdvancer.h"
#include"uilayer/StatusBar.h"
#include"uilayer/HoverCard.h"
#include"uilayer/CombatLog.h"


namespace G {
	const int CardSelectorId = 0;
	const int CardSelectionId = 1;
	const int AbilitySelectorId = 2;
	const int EnemyAdvancerId = 3;
	const int StatusBarId = 4;
	const int InitiativeTrackerId = 5;
	const int HoverCardId = 99;
	const int CombatLogId = 100;
};

Gloom::Gloom() 
	: director(level, [this](auto eventId) {this->OnDirectorEvent(eventId); }), cardGenerator(level)
{
}

Gloom::~Gloom()
{
}

void Gloom::Initialize()
{
	Icons::Load(Services().Textures());
	InitializeUI();
	
	level.LoadMap("levels/scenario_001.json");
	level.SpawnRoom(1);

	camera.SetPositionCenter( level.Center() );

	Render::SetLightData();
}

void Gloom::Deinitialize()
{
	DeinitializeUI();
	Icons::Unload();
}

void Gloom::Resize()
{
	Log::Info("CardRendering", "Resize");
	const auto& windowSize = WindowState::Size();
	glm::vec2 newWindowSize{ windowSize.x, windowSize.y };

	Activity::Resize();
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

	auto inputHandled = UpdateUI(static_cast<float>(deltaTime));

	glm::vec2 cameraMouse = camera.ScreenToViewCoords(input.GetMousePositionNormalized());
	level.Update(cameraMouse);
	director.Update(input);

	auto hoverTarget = level.HasHoverTarget() ? &level.GetHoverTarget() : nullptr;
	hoverTarget = inputHandled ? nullptr : hoverTarget;

	auto hoverCard = GetViewById<HoverCard>(G::HoverCardId);
	hoverCard->SetHoverTile(level, hoverTarget);

	if(hoverTarget != nullptr && hoverTarget->IsOccupied() && director.Status() == DirectorStatus::RoundStarted ) {
		auto occupantId = hoverTarget->OccupiedId();
		auto occupant = level.ActorById(occupantId);

		if(occupant != nullptr) {
			auto enemy = dynamic_cast<Enemy*>(occupant);

			if(enemy != nullptr) {
				auto er = director.GetEnemyRound();
				hoverCard->SetEnemyRound(er);
			}
			else hoverCard->NoEnemyRound();
		}
		else hoverCard->NoEnemyRound();
	}
	else hoverCard->NoEnemyRound();



	auto combatLog = GetViewById<CombatLog>(G::CombatLogId);
	if(input.KeyDown(VK_TAB)) {
		if(combatLog->Active() == false) {
			combatLog->Activate();
			combatLog->Invalidate();
		}
	}
	else if( combatLog->Active() ) 
		combatLog->Deactivate();
}

void Gloom::Render()
{
	const auto& text = Services().Text();

	Render::EnableLight();

	camera.Push();
		level.Render(text);
		director.Render();
	camera.Pop();

	Render::NoLight();

	glPushMatrix();
		director.RenderUI(text);
	glPopMatrix();

	RenderUI();
}

void Gloom::InitializeUI()
{
	auto cardSelector = vnew CardSelect(*cardGenerator.PlayerCards(), *Icons::GetPlayerCard(), [this](const std::string & cardName) {
		
		auto deck = cardGenerator.PlayerCards();
		auto& hand = deck->Hand();

		auto cardFound = std::find_if(hand.begin(), hand.end(), [&cardName](auto playerCard) { if (playerCard->Name().compare(cardName) == 0) return true; return false; });
		if (cardFound == hand.end())
			throw "card not found";

		auto cardSelection = GetViewById<CardSelection>(1);
		if (cardSelection == nullptr)
			throw "layer not found";

		cardSelection->AddCard(**cardFound);
		});
	cardSelector->SetSize(0, 150.0f);
	cardSelector->SetAnchor(UIView::WindowAnchor::BOTTOM | UIView::WindowAnchor::LEFT | UIView::WindowAnchor::RIGHT);
	cardSelector->Activate();
	AddView(cardSelector, G::CardSelectorId);

	auto cardSelection = vnew CardSelection(*Icons::GetPlayerCard(), [this](CardSelection & cs, int eventId) {
		auto cardName0 = cs.Card(0);
		auto cardName1 = cs.Card(1);

		cs.Deactivate();

		auto cardSelector = GetViewById<CardSelect>(0);
		cardSelector->Deactivate();

		auto deck = cardGenerator.PlayerCards();
		auto& hand = deck->Hand();

		auto abilitySelector = GetViewById<AbilitySelector>(2);
		auto playerCard0 = std::find_if(hand.begin(), hand.end(), [&cardName0](auto playerCard) { if (playerCard->Name().compare(cardName0) == 0) return true; return false; });
		auto playerCard1 = std::find_if(hand.begin(), hand.end(), [&cardName1](auto playerCard) { if (playerCard->Name().compare(cardName1) == 0) return true; return false; });

		abilitySelector->SetCards(*playerCard0, *playerCard1);
		abilitySelector->Activate();

		auto initiativeCard = *playerCard0;
		level.GetPlayer()->Initiative(initiativeCard->Initiative());

		director.StartRound();
	});
	cardSelection->SetSize(0, 0);
	cardSelection->Activate();
	AddView(cardSelection, 1);


	auto abilitySelector = vnew AbilitySelector(*Icons::GetPlayerCard(), [this](auto eventId, auto abilityEnum) {
		if (eventId == 0) {
			director.EndPlayerTurn();
		}
		else {
			auto abilitySelector = GetViewById<AbilitySelector>(2);
			if (eventId == 1) {		

				switch (abilityEnum) {
				case AbilitySelector::ABILITY_DEFAULT_TOP:
					director.SetPlayerRound( cardGenerator.GetDefaultTop( *level.GetPlayer() ) );
					break;
				case AbilitySelector::ABILITY_DEFAULT_BOTTOM:
					director.SetPlayerRound(cardGenerator.GetDefaultBottom(*level.GetPlayer()));
					break;
				case AbilitySelector::ABILITY_TOP: {
					director.SetPlayerRound(cardGenerator.GetTopAction(*level.GetPlayer(), abilitySelector->GetCardName(0)));
					break;
				}
				case AbilitySelector::ABILITY_BOTTOM: {
					director.SetPlayerRound(cardGenerator.GetBottomAction(*level.GetPlayer(), abilitySelector->GetCardName(0)));
					break;
				}
				default: 
					throw "Default";
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
				case AbilitySelector::ABILITY_TOP: {
					director.SetPlayerRound(cardGenerator.GetTopAction(*level.GetPlayer(), abilitySelector->GetCardName(1)));
					break;
				}
				case AbilitySelector::ABILITY_BOTTOM: {
					director.SetPlayerRound(cardGenerator.GetBottomAction(*level.GetPlayer(), abilitySelector->GetCardName(1)));
					break;
				}
				default: 
					throw "Default";
					break;
				}
			}			
		}
	});
	abilitySelector->Deactivate();
	AddView(abilitySelector, 2);

	auto enemyAdvancer = vnew EnemyAdvancer([this]() {
		director.AdvanceEnemy();
	});
	AddView(enemyAdvancer, 3);

	auto statusBar = vnew StatusBar();
	AddView(statusBar, 4);

	auto initiativeTrackerUI = vnew InitiativeTrackerUI(director.GetInitiativeTracker());
	AddView(initiativeTrackerUI, 5);


	AddView(vnew HoverCard(), G::HoverCardId);
	AddView(vnew CombatLog(level), G::CombatLogId);
}

void Gloom::OnDirectorEvent(DirectorEvent eventId)
{
	switch (eventId) {
		case DirectorEvent::EndOfRound: {
			auto cs = GetViewById<CardSelect>(G::CardSelectorId);
			cs->Activate();
			cs->Invalidate();

			auto selection = GetViewById<CardSelection>(G::CardSelectionId);
			selection->Activate();
		
			auto deck = cardGenerator.PlayerCards();
			deck->Discard(selection->Card(0));
			deck->Discard(selection->Card(1));
			selection->ClearCards();

			GetViewById(G::AbilitySelectorId)->Deactivate();
			GetViewById(G::EnemyAdvancerId)->Deactivate();

			auto statusbar = GetViewById<StatusBar>(G::StatusBarId);
			statusbar->SetStatusText("End of Round");
			statusbar->NextRound();

			level.GetPlayer()->EndOfRoundActions();
			break;
		}
		case DirectorEvent::EnemyTurn: {
			GetViewById(G::AbilitySelectorId)->Deactivate();
			auto ea = GetViewById<EnemyAdvancer>(G::EnemyAdvancerId);

			ea->Activate();
			ea->SetEnemyActions(director.GetEnemyRound());

			GetViewById<StatusBar>(G::StatusBarId)->SetStatusText("AI Turn");
			break;
		}
		case DirectorEvent::PlayerTurn:{

			GetViewById(G::AbilitySelectorId)->Activate();
			GetViewById(G::EnemyAdvancerId)->Deactivate();

			GetViewById<StatusBar>(G::StatusBarId)->SetStatusText("Player Turn");
			break;
		}
		case DirectorEvent::AdvanceEnemy: {
			GetViewById<EnemyAdvancer>(G::EnemyAdvancerId)->Advance();
			break;
		}
	}
}
