#include"pch.h"
#include "icons.h"
#include"../textures.h"

#include <cctype>
#include <clocale>

std::map<std::string, Texture> Icons::icons;
Texture Icons::texturePlayerCard;

void Icons::Load(TextureService& textures)
{
	icons["attack"] = textures.LoadTexture("textures/icons/attack.png");
	icons["move"] = textures.LoadTexture("textures/icons/move.png");
	icons["push"] = textures.LoadTexture("textures/icons/push.png");
	icons["pull"] = textures.LoadTexture("textures/icons/pull.png");
	icons["disarm"] = textures.LoadTexture("textures/icons/disarm.png");
	icons["range"] = textures.LoadTexture("textures/icons/range.png");
	icons["poison"] = textures.LoadTexture("textures/icons/poison.png");
	icons["shield"] = textures.LoadTexture("textures/icons/shield.png");

	texturePlayerCard = textures.LoadTexture("textures/player_ability_back.png");

	icons["monsterabilitycardback"] = textures.LoadTexture("textures/monster_ability_card_back.png");

	icons["bandit guard"] = textures.LoadTexture("textures/monsters/Horz-Bandit Guard.png");
	icons["living bones"] = textures.LoadTexture("textures/monsters/Horz-Living Bones.png");
	icons["bandit archer"] = textures.LoadTexture("textures/monsters/Horz-Bandit Archer.png");
	icons["bandit guard elite"] = textures.LoadTexture("textures/monsters/Horz-Bandit Guard_elite.png");
	icons["living bones elite"] = textures.LoadTexture("textures/monsters/Horz-Living Bones_elite.png");
	icons["bandit archer elite"] = textures.LoadTexture("textures/monsters/Horz-Bandit Archer_elite.png");

	icons["trap"] = textures.LoadTexture("textures/monsters/Spike Trap 1h.png");
	icons["obstacle"] = textures.LoadTexture("textures/monsters/Crate B 1h.png");
	icons["coin"] = textures.LoadTexture("textures/monsters/Coin.png");
	icons["treasure"] = textures.LoadTexture("textures/monsters/Treasure Tile 1h - Horiz.png");
	icons["door (h)"] = textures.LoadTexture("textures/monsters/Stone Door Closed 1h - Horizontal.png");
	icons["door (v)"] = textures.LoadTexture("textures/monsters/Stone Door Closed 1h - Vertical.png");

	icons["start"] = textures.LoadTexture("textures/icons/icon_start.png");

	icons["element_fire"] = textures.LoadTexture("textures/icons/icon_fire.png");
	icons["element_ice"] = textures.LoadTexture("textures/icons/icon_ice.png");
	icons["element_wind"] = textures.LoadTexture("textures/icons/icon_wind.png");
	icons["element_light"] = textures.LoadTexture("textures/icons/icon_light.png");
	icons["element_dark"] = textures.LoadTexture("textures/icons/icon_dark.png");
	icons["element_earth"] = textures.LoadTexture("textures/icons/icon_earth.png");
	icons["element_any"] = textures.LoadTexture("textures/icons/icon_any_element.png");
	icons["element_spend"] = textures.LoadTexture("textures/icons/icon_spend_element.png");
}

void Icons::Unload()
{
}

Texture* Icons::Get(const std::string& name)
{
	auto name_dest = std::string();
	name_dest.resize(name.size());
	std::transform(name.begin(), name.end(), name_dest.begin(), std::tolower);

	auto  it = icons.find(name_dest);

	if (it != icons.end())
		return &(it->second);

	return nullptr;
}

Texture* Icons::GetPlayerCard()
{
	return &texturePlayerCard;
}
