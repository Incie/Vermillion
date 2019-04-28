#include"pch.h"
#include "icons.h"
#include"../textures.h"

std::map<std::string, Texture> Icons::icons;

void Icons::Load(TextureService& textures)
{
	icons["Attack"] = textures.LoadTexture("textures/icons/attack.png");
	icons["Move"] = textures.LoadTexture("textures/icons/move.png");
	icons["PUSH"] = textures.LoadTexture("textures/icons/push.png");
	icons["PULL"] = textures.LoadTexture("textures/icons/pull.png");
	icons["DISARM"] = textures.LoadTexture("textures/icons/disarm.png");
	icons["Range"] = textures.LoadTexture("textures/icons/range.png");
	icons["POISON"] = textures.LoadTexture("textures/icons/poison.png");
	icons["Shield"] = textures.LoadTexture("textures/icons/shield.png");
}

void Icons::Unload()
{
}

Texture* Icons::Get(const std::string& name)
{
	auto  it = icons.find(name);

	if (it != icons.end())
		return &(it->second);

	return nullptr;
}
