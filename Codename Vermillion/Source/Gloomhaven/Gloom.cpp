#include"pch.h"
#include "Gloom.h"
#include"GL/glew.h"
#include"glm/glm.hpp"
#include<vector>
#include<Windows.h>
#include"level/Hexagon.h"
#include"level/Level.h"
#include"fmt/format.h"

#include"entity/Entity.h"

#include"action/ActionAttack.h"
#include"action/ActionMove.h"

#include"enemyai/EnemyAI.h"
#include"enemyai/EnemyAction.h"
#include"enemyai/EnemyRound.h"

EnemyRound *enemyRound = nullptr;

Gloom::Gloom() 
	: action(nullptr), enemyAi(level)
{
}

Gloom::~Gloom()
{
	if (action)
		delete action;
}

void Gloom::Initialize()
{
	level.Generate();
	level.Spawn();

	enemyRound = new EnemyRound();
	enemyRound->AddAction(new EnemyMove(level, 2));
	enemyRound->AddAction(new EnemyAttack(level, 2, 1));

	enemyAi.SetActor(level.ActorById(2));
	enemyAi.SetRoundActions(enemyRound);
}

void Gloom::Deinitialize()
{
	delete enemyRound;
}

void Gloom::Update(double deltaTime)
{
	const auto& input = Services().Input();

	if (action == nullptr && input.KeyOnce(VK_F1)) {
		action = new ActionMove(level, *level.GetPlayer(), 6);
	}

	if (action == nullptr && input.KeyOnce(VK_F2)) {
		action = new ActionAttack(level, *level.GetPlayer(), 1, 4, 1);
	}

	if (input.KeyOnce(VK_F3))
		level.ShowCoords(true);

	if (input.KeyOnce('N')) {
		enemyAi.Step();
	}

	if (input.KeyOnce('B'))
		camera.SetPositionTopLeft(glm::vec2());

	if (input.KeyDown(VK_RBUTTON)) {
		const auto& deltaMouse = input.GetMouseDelta();
		camera.Move(deltaMouse);
	}

	if (input.KeyDown(VK_ADD)) camera.ZoomByFactor(0.9f);
	if (input.KeyDown(VK_SUBTRACT)) camera.ZoomByFactor(1.1f);


	glm::vec2 cameraMouse;
	cameraMouse = camera.ScreenToViewCoords(input.GetMousePositionNormalized());

	level.Update(cameraMouse);

	if (action != nullptr && input.KeyOnce(VK_LBUTTON)) {
		if (level.HasHoverTarget())
			action->Click(level.GetHoverTarget().Location() );
	}

	if (action != nullptr && input.KeyOnce(VK_BACK)) {
		action->Undo();
	}

	if( action != nullptr && input.KeyOnce(VK_ESCAPE) ){
		action->Reset();
		delete action;
		action = nullptr;
		level.ClearHighlights();
	}

	if (action != nullptr && input.KeyOnce(VK_RETURN)) {
		if( action->Perform(*level.GetPlayer()) ) {
			delete action;
			action = nullptr;
		}
	}
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

	camera.Push();
		glPushMatrix();
			glBegin(GL_LINES);
				glVertex2f(0, -500); glVertex2f(0, 500);
				glVertex2f(-500, 0); glVertex2f(500, 0);
			glEnd();
		glPopMatrix();

		level.Render( Services().Text() );

		if (action)
			action->Render();

		enemyRound->Render();
	camera.Pop();

	glPushMatrix();
		glTranslatef(5.0f, 300.0f, 0.0f);
		enemyRound->RenderRoundCard(Services().Text());
	glPopMatrix();

	if (level.combatLog.size() > 0) {
		glPushMatrix();
		glTranslatef(0, 500.0f, 0);
		for (const auto& line : level.combatLog) {
			Services().Text().PrintLine(0, 0, line, 20, Colorf(1));
		}
		glPopMatrix();
	}

	if (level.HasHoverTarget()) {
		const auto& hoverTile = level.GetHoverTarget();
		
		if (hoverTile.IsOccupied()) {
			const auto hoverActor = level.ActorById(hoverTile.OccupiedId());
			glPushMatrix();
			glTranslatef(5, 50, 0);
			hoverActor->PrintStats(Services().Text() );
			glPopMatrix();
		}
	}

	if (action != nullptr) {
		Services().Text().Print(200, 25, action->Description(), 20, Colorf(1, 1, 1));
	}
}
