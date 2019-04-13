#include"pch.h"
#include "Gloom.h"
#include"GL/glew.h"
#include"glm/glm.hpp"
#include<vector>
#include<Windows.h>
#include"../Utils/Hexagon.h"
#include"fmt/format.h"
#include "Level.h"

#include"ActionMove.h"

Gloom::Gloom() : action(nullptr)
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
}

void Gloom::Deinitialize()
{
}

void Gloom::Update(double deltaTime)
{
	const auto& input = Services().Input();

	if (action == nullptr && input.KeyOnce(VK_F1)) {
		//action = new ActionMove(level, level.playerHex.cubeCoordinate, 3);
	}

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
	}

	if (action != nullptr && input.KeyOnce(VK_RETURN)) {
		//action->Perform(level.playerHex);
		delete action;
		action = nullptr;
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
	camera.Pop();

	if (action != nullptr) {
		Services().Text().Print(200, 25, "MOVING", 20, Colorf(1, 1, 1));
	}
}
