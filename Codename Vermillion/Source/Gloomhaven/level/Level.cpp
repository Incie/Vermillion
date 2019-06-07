#include"pch.h"
#include "Level.h"
#include"GL/glew.h"
#include"fmt/format.h"
#include"../../Framework/services.h"
#include"../level/Spawner.h"
#include"../entity/Entity.h"

Level::Level() 
	: distance(), selectedCoord(), hoverTarget(nullptr), spawner(*this), hexagonSize(50.0f)
{
	showCoords = false;
	hasDistance = false;
	selectedHex = nullptr;
}

Level::~Level()
{
	for (auto tile : tiles)
		delete tile;
	tiles.clear();

	for (auto entity : entities)
		delete entity;
	entities.clear();

}

#include"nholmann-json/json.hpp"
#include"..//file.h"
void Level::LoadMap(const std::string& fileName)
{
	auto fileContent = FileReader::ReadFileContent(fileName);

	using json = nlohmann::json;
	json j = json::parse(fileContent);

	auto jTiles = j["tiles"];

	float size = hexagonSize;
	float width = 2.0f * size;
	float height = sqrtf(3.0f) * size;

	for(int i = 0; i < jTiles.size(); ++i) {
		auto jTile = jTiles[i];
		auto x = jTile.value("x", -1);
		auto y = jTile.value("y", -1);
		auto entityName = jTile.value("entity", "None");
		

		float hx = (3.0f / 4.0f) * width * static_cast<float>(x);
		float hy = -static_cast<float>(y) * height - (1.0f / 2.0f) * height * static_cast<float>(x);
		auto tile = vnew Tile(glm::ivec3(x, y, -(x + y)), glm::vec3(hx, hy, 0.0f));
		tile->GetHexagon().Generate(glm::vec2(hx, hy), size * 0.9f, size);
		tile->Disable();
		tile->RoomNumber(jTile.value("room", 1));
		tiles.push_back(tile);


		if(!(entityName == "None" || entityName == "")) {
			spawner.AddEntitySpawn(entityName, tile->Location(), tile->RoomNumber() );
		}
	}
}

void Level::Generate()
{
	double size = 50.0;
	double width = 2.0 * size;
	double height = sqrt(3.0) * size;

	auto nX = 7;
	auto nY = 5;

	auto startX = 1;
	auto startY = 0;

	glm::ivec3 coord = glm::ivec3(0, 0, 0);
	for (int y = startY; y < nY; ++y) {
		auto h = height * (double)y;

		coord.x = startX;
		coord.y = -y;
		coord.z = y;

		for (int x = startX; x < nX + startX; ++x) {
			auto w = (3.0 / 4.0) * width * (double)x;

			double add = 0.0;
			if (x % 2 == 1)
				add = 0.5 * height;

			tiles.push_back( vnew Tile(coord, glm::vec3(w, h+add, 0.0f)) );
			auto &tile = *tiles.back();
			tile.GetHexagon().Generate(glm::vec2(w, h + add), 40, 50);

			coord.x++;
			if (x % 2 == 0) 
				coord.y--;
			coord.z = -(coord.x + coord.y);
		}
	}
}

void Level::SpawnRoom(int roomNumber)
{
	for(auto tile : tiles) {
		if(tile->RoomNumber() == roomNumber)
			tile->Enable();
	}
	
	spawner.SpawnRoom(roomNumber);
}

void Level::Update(const glm::vec2& cameraMouse)
{
	Tile* closestTile = nullptr;
	float closestDistance = 10e10f;
	for (auto tile : tiles) {
		
		auto distFromCenter = tile->DistanceFromCenterTo(cameraMouse);
		if (distFromCenter < 50.0) {
			if (distFromCenter < closestDistance) {
				closestTile = tile;
				closestDistance = (float)distFromCenter;
			}
		}
	}

	hoverTarget = nullptr;
	if (closestTile != nullptr) {
		hoverTarget = closestTile;
	}
}

void Level::Render(const TextService& text)
{
	glDisable(GL_TEXTURE_2D);

	for(auto tile : tiles) {
		if(tile->Enabled()) {
			tile->GetHexagon().Render();

			for(auto ce : tile->ContainingEntities()) {

				if( ce->Active() )
					ce->RenderModel().Render();
			}
		}
	}
	
	for(auto entity : entities) {
		if(!entity->Active())
			continue;

		entity->Render(text);
	}

	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);

	for(auto tile : tiles) {
		auto& hex = tile->GetHexagon();

		glPushMatrix();
			glTranslatef(tile->WorldPosition().x - 20.0f, tile->WorldPosition().y, tile->WorldPosition().z);

			if (showCoords) {
				const auto& coord = tile->Location();
				text.Print(0, 0, fmt::format("{0},{1},{2}", coord.x, coord.y, coord.z), 16, Colors::Black);
			}
		glPopMatrix();
	}
}

void Level::Highlight(const glm::ivec3& center, int range, const glm::vec3& highlightColor, std::function<bool(const Tile&)> highlightPredicate)
{
	for (auto& tile : tiles) {
		auto& hex = tile->GetHexagon();
		auto distance = tile->DistanceTo(center);

		if (distance > range)
			continue;

		if(highlightPredicate(*tile) )
			hex.SetHighlight(highlightColor);
	}
}

void Level::Highlight(const glm::ivec3& center, int range, const glm::vec3& highlightColor, bool ignoreOccupied)
{
	for (auto& tile : tiles) {
		auto& hex = tile->GetHexagon();
		auto distance = tile->DistanceTo(center);

		if (tile->IsOccupied() && tile->DistanceTo(center) != 0 && !ignoreOccupied)
			continue;

		if (distance <= range)
			hex.SetHighlight(highlightColor);
	}
}

void Level::ClearHighlights()
{
	for (auto& tile : tiles)
		tile->GetHexagon().NoHighlight();
}

std::vector<Tile*> Level::TilesWithin(const glm::ivec3& center, int range)
{
	auto tilesWithin = std::vector<Tile*>();
	for (auto tile : tiles ) {
		auto distance = tile->DistanceTo(center);
		if (distance == 0)
			continue;

		if (distance <= range)
			tilesWithin.push_back(tile);
	}

	return tilesWithin;
}

Tile& Level::TileOccupiedBy(int entityId)
{
	for (auto& tile : tiles) {
		if (!tile->IsOccupied())
			continue;
		if (tile->OccupiedId() == entityId)
			return *tile;
	}

	throw "TileOccupiedBy() -> Tile Not Found";
}

Tile& Level::TileAt(const glm::ivec3& location)
{
	for (auto& tile : tiles) {
		auto& hex = tile->GetHexagon();
		if (tile->Location().x == location.x && tile->Location().y == location.y && tile->Location().z == location.z) {
			return *tile;
		}
	}

	return *tiles[0];
}

void Level::RemoveActorById(int actorId)
{
	auto actor = ActorById(actorId);
	auto& tile = TileOccupiedBy(actorId);

	tile.SetOccupied(-1);
}

Actor* Level::ActorById(int actorId)
{
	for (auto entity : entities)
	{
		if (entity->EntityId() == actorId)
			return dynamic_cast<Actor*>(entity);
	}

	return nullptr;
}

Actor* Level::GetPlayer()
{
	for (auto entity : entities) {
		auto player = dynamic_cast<Player*>(entity);

		if (player != nullptr)
			return player;
	}

	return nullptr;
}

std::vector<Actor*> Level::Monsters()
{
	auto monsters = std::vector<Actor*>();

	for (auto entity : entities) {
		auto monster = dynamic_cast<Enemy*>(entity);

		if (monster != nullptr && monster->Health() > 0 && monster->Active() )
			monsters.push_back(monster);

	}

	return monsters;
}

void Level::AddEntity(Entity* entity)
{
	entities.push_back(entity);
}

void Level::ShowCoords(bool value)
{
	showCoords = value;
}

glm::vec2 Level::Center()
{
	glm::vec2 min(9999), max(-9999);

	for(auto tile : tiles) {
		if(tile->Enabled()) {
			const auto& wp = tile->WorldPosition();
			min.x = std::min<float>(min.x, wp.x);
			min.y = std::min<float>(min.y, wp.y);
			
			max.x = std::max<float>(max.x, wp.x);
			max.y = std::max<float>(max.y, wp.y);
		}
	}

	return min + (max - min) * 0.5f;
}

glm::ivec3 Level::GetCenterCoord(int x, int y, int z)
{
	for (auto tile : tiles) {
		auto& hex = tile->GetHexagon();
		if (tile->Location().x == x && tile->Location().y == y && tile->Location().z == z) {
			return tile->WorldPosition();
		}
	}

	return glm::ivec3();
}

//if (input.KeyOnce(VK_LBUTTON) && closestHex != nullptr)
//	selectedHex = closestHex;

//if (selectedHex != nullptr)
//	selectedHex->SetColor(glm::vec3(0, 1, 0));

//hasDistance = false;
//if (selectedHex != nullptr && closestHex != nullptr) {
//	distance = closestHex->cubeCoordinate - selectedHex->cubeCoordinate;
//	hasDistance = true;
//}

//if (input.KeyOnce(VK_SPACE)) {
//	if (closestHex != nullptr) {
//		playerHex.center = closestHex->center;
//		playerHex.center.z = 5.0f;
//	}
//}


	//if (hasDistance) {
	//	int hexDistance = abs(distance.x) + abs(distance.y) + abs(distance.z);
	//	text.Print(0, 50, fmt::format("{0},{1},{2} = {3}", distance.x, distance.y, distance.z, hexDistance / 2), 16, Colorf(1));
	//}