#include"pch.h"
#include "icons.h"
#include"../textures.h"

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
