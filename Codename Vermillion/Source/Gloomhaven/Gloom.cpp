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


Gloom::Gloom() 
	: director(level)
{
}

Gloom::~Gloom()
{
}

void Gloom::Initialize()
{
	level.Generate();
	level.Spawn();
}

void Gloom::Deinitialize()
{
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

	if (level.combatLog.size() > 0) {
		glPushMatrix();
		glTranslatef(0, 500.0f, 0);
		for (const auto& line : level.combatLog) {
			text.PrintLine(0, 0, line, 20, Colorf(1));
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
