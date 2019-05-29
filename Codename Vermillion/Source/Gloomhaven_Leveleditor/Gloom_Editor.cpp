#include"pch.h"
#include "Gloom_Editor.h"
#include"..//Gloomhaven/level/Hexagon.h"
#include"..//camera2d.h"
#include"..//Gloomhaven/level/Tile.h"

GloomEditor::GloomEditor()
{
}

GloomEditor::~GloomEditor()
{
}

std::vector<Tile*> hexes;
Camera2D camera;

void GloomEditor::Initialize()
{
	double size = 10.0;
	double width = 2.0 * size;
	double height = sqrt(3.0) * size;

	for( int y = 0; y < 100; ++y ){
		float fy = static_cast<float>(y);
		for( int x = 0; x < 100; ++x ){
			float fx = static_cast<float>(x);	

			float hx = (3.0f / 4.0f) * width * fx;
			float hy = -fy * height - (1.0f / 2.0f) * height * fx;

			hexes.push_back(new Tile(glm::ivec3(x, y, -(x+y)), glm::vec3(hx, hy, 0.0f)));
			auto& hex = hexes.back()->GetHexagon();
			hex.Generate(glm::vec2(hx, hy), size * 0.75f, size);
		}
	}
}

void GloomEditor::Deinitialize()
{
}

#include<Windows.h>
void GloomEditor::Update(double delta)
{
	auto& input = Services().Input();

	if (input.KeyDown(VK_RBUTTON)) {
		const auto& deltaMouse = input.GetMouseDelta();
		camera.Move(deltaMouse);
	}
}

#include"GL/glew.h"
void GloomEditor::Render()
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
		auto& text = Services().Text();
		for( auto& h : hexes ){
			h->GetHexagon().Render();
		}

		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0);

		for(auto tile : hexes ) {
			auto& hex = tile->GetHexagon();

			//glPushMatrix();
			//	glTranslatef(tile->WorldPosition().x - 20.0f, tile->WorldPosition().y, tile->WorldPosition().z);
			//	const auto& coord = tile->Location();
			//	text.Print(0, 0, fmt::format("{0},{1},{2}", coord.x, coord.y, coord.z), 16, Colorf(0, 0, 0));
			//glPopMatrix();
		}
	camera.Pop();
}
