#include "Gloom.h"
#include"GL/glew.h"
#include"glm/glm.hpp"
#include<vector>
#include<Windows.h>
#include"../Utils/Hexagon.h"
#include"fmt/format.h"


Gloom::Gloom()
{
}

Gloom::~Gloom()
{
}

Hexagon playerHex = Hexagon(glm::vec2(), 25, 15);

std::vector<Hexagon> hexGrid;
void Gloom::Initialize()
{
	double size = 50.0;
	double width = 2.0 * size;
	double height = sqrt(3.0) * size;

	auto nY = 10;
	auto nX = 10;

	auto start = 0;

	glm::ivec3 coord = glm::ivec3(0, 0, 0);
	for (int y = start; y < nY; ++y) {
		auto h = height * (double)y;

		coord.x = 0;
		coord.y = -y;
		coord.z = y;

		for (int x = start; x < nX; ++x) {
			auto w = (3.0 / 4.0) * width * (double)x;

			double add = 0.0;
			if (x % 2 == 1)
				add = 0.5 * height;

			auto hex = Hexagon(glm::vec2(w, h + add), 40, 50);
			hex.cubeCoordinate = coord;
			hexGrid.push_back(hex);


			coord.x++;
			if (x % 2 == 0) coord.y--;
			coord.z = -(coord.x + coord.y);
		}
	}


	playerHex.center = hexGrid[0].center;
	playerHex.center.z = 15.0f;
	playerHex.SetColor(glm::vec3(0.2f, 0.15f, 0.88f));
}

void Gloom::Deinitialize()
{
}


Hexagon *selectedHex = nullptr;
glm::ivec3 selectedCoord;

bool hasDistance = false;
glm::ivec3 distance;

void Gloom::Update(double deltaTime)
{
	const auto& input = Services().Input();

	if (input.KeyDown(VK_RBUTTON)) {
		const auto& deltaMouse = input.GetMouseDelta();
		camera.Move(deltaMouse);
	}

	if (input.KeyDown(VK_ADD)) camera.ZoomByFactor(0.9);
	if (input.KeyDown(VK_SUBTRACT)) camera.ZoomByFactor(1.1);

	glm::vec2 cameraMouse;
	cameraMouse = camera.ScreenToViewCoords(input.GetMousePositionNormalized());


	Hexagon* closestHex = nullptr;
	float closestDistance = 10e10;
	for (auto& hex : hexGrid) {
		hex.SetColor(glm::vec3(1, 1, 1));
		auto distFromCenter = hex.DistanceFromCenterTo(cameraMouse);
		if (distFromCenter < 50.0) {
			if (distFromCenter < closestDistance) {
				closestHex = &hex;
				closestDistance = distFromCenter;
			}
		}
	}

	if( closestHex != nullptr )
		closestHex->SetColor(glm::vec3(0.5, 0, 0));

	if (input.KeyOnce(VK_LBUTTON) && closestHex != nullptr )
		selectedHex = closestHex;

	if( selectedHex != nullptr )
		selectedHex->SetColor(glm::vec3(0, 1, 0));

	hasDistance = false;
	if (selectedHex != nullptr && closestHex != nullptr) {
		distance = closestHex->cubeCoordinate - selectedHex->cubeCoordinate;
		hasDistance = true;
	}

	if (input.KeyOnce(VK_SPACE)) {
		if (closestHex != nullptr) {
			playerHex.center = closestHex->center;
			playerHex.center.z = 5.0f;
		}
	}
}

void Gloom::Render()
{
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);

	float ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	float diffuseLight[] = { 0.8f, 0.8f, 0.8, 1.0f };
	float specularLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	glm::vec4 lightDirection = glm::vec4(0.355336f, 0.906561, -0.227779, 0.0);

	glLightfv(GL_LIGHT0, GL_POSITION, &lightDirection.x);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);


	camera.Push();
		for (auto h : hexGrid)
			h.Render();

		glPushMatrix();
			glTranslatef(playerHex.center.x, playerHex.center.y, playerHex.center.z);
			playerHex.Render();
		glPopMatrix();

	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);

	auto& text = Services().Text();
	/*for (auto h : hexGrid) {
		glPushMatrix();
			glTranslatef(h.center.x - 20.0f, h.center.y, h.center.z);
			text.Print(0, 0, fmt::format("{0},{1},{2}", h.cubeCoordinate.x, h.cubeCoordinate.y, h.cubeCoordinate.z), 16, Colorf(0,0,0));
		glPopMatrix();
	}*/

	camera.Pop();

	if (hasDistance) {
		int hexDistance = abs(distance.x) + abs(distance.y) + abs(distance.z);
		text.Print(0, 50, fmt::format("{0},{1},{2} = {3}", distance.x, distance.y, distance.z, hexDistance / 2), 16, Colorf(1));
	}


}
