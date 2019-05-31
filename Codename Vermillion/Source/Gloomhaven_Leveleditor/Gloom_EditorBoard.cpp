#include"pch.h"
#include"Gloom_EditorBoard.h"
#include"glm/glm.hpp"
#include"..//Gloomhaven/level/Hexagon.h"

#include"..//services.h"

EditorBoard::EditorBoard()
	: hover(nullptr), selected(nullptr), hexagonSize(50.0f)
{
	mode = 0;
}

EditorBoard::~EditorBoard()
{
	hover = nullptr;
	selected = nullptr;

	Delete();
}

void EditorBoard::Delete()
{
	for(auto h : tiles)
		::delete h;
	tiles.clear();
}

void EditorBoard::SetCallback(std::function<void(EditorTile*)> callback)
{
	this->callback = callback;
}

void EditorBoard::SetMode(int mode)
{
	this->mode = mode;

	if (selected != nullptr) {
		selected->GetHexagon().SetColor(glm::vec3(1, 1, 1));
		selected = nullptr;
	}
}

void EditorBoard::Generate(int boardWidth, int boardHeight)
{
	float size = hexagonSize;
	float width = 2.0f * size;
	float height = sqrtf(3.0f) * size;

	int tileCount = boardWidth * boardHeight;
	tiles.reserve(tileCount);

	for (int y = 0; y < boardWidth; ++y)
	{
		float fy = static_cast<float>(y);
		for (int x = 0; x < boardHeight; ++x) 
		{
			float fx = static_cast<float>(x);

			float hx = 0.75f * width * fx;
			float hy = -fy * height - 0.5f * height * fx;

			auto editorTile = vnew EditorTile(glm::ivec3(x, y, -(x + y)), glm::vec3(hx, hy, 0.0f));
			tiles.push_back(editorTile);
			auto tile = tiles.back();

			tile->Disable();
			auto& hex = tile->GetHexagon();
			hex.Generate(glm::vec2(hx, hy), size * 0.75f, size);
			hex.SetHighlight(glm::vec3(0.1f));
		}
	}
}

#include<Windows.h>
void EditorBoard::Clear()
{
	if (hover != nullptr) {
		if (hover->Enabled())
			hover->GetHexagon().NoHighlight();
		else hover->GetHexagon().SetHighlight(glm::vec3(0.1f));
	}
}
void EditorBoard::Update(const InputService& input, glm::vec2& viewCoords)
{
	if (hover != nullptr) {
		if (hover->Enabled())
			hover->GetHexagon().NoHighlight();
		else hover->GetHexagon().SetHighlight(glm::vec3(0.1f));
	}

	float closest = 100.0f;
	for (auto tile : tiles) {
		float d = (float)tile->DistanceFromCenterTo(viewCoords);
		if (d < closest)
		{
			closest = d;
			hover = tile;
		}
	}

	if (closest > hexagonSize)
		hover = nullptr;


	if (hover != nullptr) {
		hover->GetHexagon().SetHighlight(glm::vec3(1, 0.1f, 0.2f));

		if (hover->Enabled() == false)
			hover->GetHexagon().SetHighlight(glm::vec3(0.3f, 0, 0));

		if (input.KeyDown(VK_LBUTTON)) {

			if( mode == 0 ){
				hover->Enable();

				if (input.KeyDown(VK_CONTROL))
					hover->Disable();
			}
			else if (mode == 1) {
				if (hover->Enabled() == false)
					return;

				callback(hover);

				if (selected != nullptr) {
					selected->GetHexagon().SetColor(glm::vec3(1, 1, 1));
				}

				selected = hover;
				selected->GetHexagon().SetColor(glm::vec3(0, 0, 1));
			}
		}
	}
}

#include"GL/glew.h"
void EditorBoard::Render(const TextService& textService)
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

	for(auto& tile : tiles) {
		tile->GetHexagon().Render();

		if(tile->entity != nullptr) {
			tile->entity->Render();
		}
	}

	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
}

#include"nholmann-json/json.hpp"
#include<fstream>

void EditorBoard::SaveToDisk()
{
	using JSON = nlohmann::json;
	JSON json;

	for(auto tile : tiles) {
		JSON jsontile;
		jsontile["x"] = tile->Location().x;
		jsontile["y"] = tile->Location().y;

		json["tiles"].push_back(jsontile);
	}

	std::string serialized = json.dump();

	std::ofstream out("levels/level1.json");
	out << serialized;
	out.close();
}

void EditorBoard::LoadFromDisk()
{
	Delete();

	std::ifstream fileStream("levels/level1.json");
	std::string serializedScenario;

	fileStream.seekg(0, std::ios::end);
	serializedScenario.reserve(fileStream.tellg());
	fileStream.seekg(0, std::ios::beg);

	serializedScenario.assign((std::istreambuf_iterator<char>(fileStream)), std::istreambuf_iterator<char>());

	using json = nlohmann::json;
	json j = json::parse(serializedScenario);

	auto jTiles = j["tiles"];

	float size = hexagonSize;
	float width = 2.0f * size;
	float height = sqrtf(3.0f) * size;

	for(int i = 0; i < jTiles.size(); ++i) {
		auto jTile = jTiles[i];
		auto x = jTile.value("x", -1);
		auto y = jTile.value("y", -1);

		float hx = (3.0f / 4.0f) * width * static_cast<float>(x);
		float hy = -static_cast<float>(y) * height - (1.0f / 2.0f) * height * static_cast<float>(x);
		auto tile = vnew EditorTile(glm::ivec3(x, y, -(x + y)), glm::vec3(hx, hy, 0.0f));
		tiles.push_back(tile);

		auto& hex = tile->GetHexagon();
		hex.Generate(glm::vec2(hx, hy), size * 0.75f, size);
		hex.SetHighlight(glm::vec3(1.0f));
	}
}
