#include"render.h"
#include"pch/pch.h"
#include"editorlevel.h"

void EditorLevel::AddPage()
{
	glm::ivec2 from{0,0};

	for(auto page : pages) {
		from.x = page->to.x;
	}

	auto page = pages.emplace_back(std::make_shared<EditorPage>(EditorPage{from, from + glm::ivec2{15, 0}}));
	CalculatePageSize(page);
}

void EditorLevel::RemovePage(const glm::ivec2& atWorldCoord)
{
}

void EditorLevel::AddTileAt(const glm::ivec2& gridCoord)
{
	if(!PageExists(gridCoord) || TileExists(gridCoord)) {
		return;
	}

	auto tile = CreateTile(gridCoord);

	auto page = GetPage(gridCoord);

	if(page) {
		page->background.push_back(tile);
		CalculatePageSize(page);
	}
}

void EditorLevel::RemoveTileAt(const glm::ivec2& gridCoord)
{
	if(!TileExists(gridCoord)) {
		return;
	}

	for(auto it = std::begin(pages); it != std::end(pages); ++it) {
		auto& page = **it;

		for(auto tileIterator = std::begin(page.background); tileIterator != std::end(page.background); ++tileIterator) {
			
			if(tileIterator->gridCoord == gridCoord) {
				page.background.erase(tileIterator);
				CalculatePageSize(*it);
				return;
			}
		}
	}
}

int EditorLevel::TileCount()
{
	size_t counts = 0;
	for(auto& page : pages) {
		counts += page->background.size();
	}

	return static_cast<int>(counts);
}

void EditorLevel::SaveLevel()
{
}

void EditorLevel::LoadLevel()
{
}

void EditorLevel::Render()
{
	for(const auto& page : pages) {
		Render::Quad(page->from * 50, (page->to - page->from) * 50, Colors::Gray);

		for(const auto& tile : page->background) {
			Render::Quads(tile.verts, tile.colors, 4);
		}
	}
}

std::shared_ptr<EditorPage> EditorLevel::GetPage(const glm::ivec2& gridCoord)
{
	for(const auto& page : pages) {
		if(page->from.x <= gridCoord.x && page->to.x > gridCoord.x) {
			return page;
		}
	}

	return std::shared_ptr<EditorPage>(nullptr);
}

bool EditorLevel::PageExists(const glm::ivec2& gridCoord)
{
	auto page = GetPage(gridCoord);

	if(page) {
		return true;
	}

	return false;
}

bool EditorLevel::TileExists(const glm::ivec2& gridCoord)
{
	for(const auto& page : pages) {
		for(const auto& tile : page->background) {
			if(tile.gridCoord == gridCoord)
				return true;
		}
	}
	return false;
}

void EditorLevel::CalculatePageSize(std::shared_ptr<EditorPage>& page)
{
	page->from = {page->from.x,9999};
	page->to = {page->to.x, -9999};

	if(page->background.size() == 0) {
		page->from.y = -9999;
		page->to.y = 9999;
		return;
	}

	for(const auto& tile : page->background) {
		page->from.y = glm::min(page->from.y, tile.gridCoord.y);
		page->to.y = glm::max(page->to.y, tile.gridCoord.y + 1);
	}
}

EditorTile EditorLevel::CreateTile(const glm::ivec2& gridCoord)
{
	EditorTile tile;
	tile.gridCoord = gridCoord;

	auto topLeftWorld = glm::vec2{gridCoord.x * 50.0f, gridCoord.y * 50.0f};

	tile.from = topLeftWorld;
	tile.to = topLeftWorld + glm::vec2{50.0f, 50.0f};
	tile.verts[0] = topLeftWorld;
	tile.verts[1] = {tile.to.x, topLeftWorld.y};
	tile.verts[2] = tile.to;
	tile.verts[3] = {topLeftWorld.x, tile.to.y};

	for(auto i : {0, 1, 2, 3}) {
		tile.colors[i] = Colors::White;
	}

	return tile;
}
