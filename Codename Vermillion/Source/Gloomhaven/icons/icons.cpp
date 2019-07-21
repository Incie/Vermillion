#include"pch.h"
#include "icons.h"
#include"../textures.h"

#include <cctype>
#include <clocale>

std::map<std::string, Texture> Icons::icons;
Texture Icons::texturePlayerCard;

void Icons::Load(TextureService& textures)
{
	Add(textures, "attack", "icons/attack.png");
	Add(textures, "move", "icons/move.png");
	Add(textures, "push", "icons/push.png");
	Add(textures, "pull", "icons/pull.png");
	Add(textures, "disarm", "icons/disarm.png");
	Add(textures, "range", "icons/range.png");
	Add(textures, "poison", "icons/poison.png");
	Add(textures, "muddle", "icons/muddle.png");
	Add(textures, "strengthen", "icons/strengthen.png");
	Add(textures, "shield", "icons/shield_outline.png");
	Add(textures, "pierce", "icons/pierce.png");
	Add(textures, "heal", "icons/heal.png");

	Add(textures, "reshuffle", "icons/reshuffle.png");
	Add(textures, "retaliate", "icons/retaliate.png");
	Add(textures, "stun", "icons/stun.png");
	Add(textures, "wound", "icons/wound.png");
	Add(textures, "invisible", "icons/invisible.png");
	Add(textures, "immobilise", "icons/immobilize.png");
	Add(textures, "target", "icons/target.png");
	Add(textures, "bless", "icons/bless.png");
	Add(textures, "curse", "icons/curse.png");
	Add(textures, "health", "icons/health.png");

	texturePlayerCard = textures.LoadTexture("textures/player_ability_back.png");

	Add(textures, "monsterabilitycardback", "monster_ability_card_back.png");
	Add(textures, "bandit guard", "monsters/Horz-Bandit Guard.png");
	Add(textures, "living bones", "monsters/Horz-Living Bones.png");
	Add(textures, "bandit archer", "monsters/Horz-Bandit Archer.png");
	Add(textures, "bandit guard elite", "monsters/Horz-Bandit Guard_elite.png");
	Add(textures, "living bones elite", "monsters/Horz-Living Bones_elite.png");
	Add(textures, "bandit archer elite", "monsters/Horz-Bandit Archer_elite.png");
	Add(textures, "trap", "monsters/Spike Trap 1h.png");
	Add(textures, "obstacle", "monsters/Crate B 1h.png");
	Add(textures, "coin", "monsters/Coin.png");
	Add(textures, "treasure", "monsters/Treasure Tile 1h - Horiz.png");
	Add(textures, "door (h)", "monsters/Stone Door Closed 1h - Horizontal.png");
	Add(textures, "door (v)", "monsters/Stone Door Closed 1h - Vertical.png");
	Add(textures, "start", "icons/icon_start.png");
	Add(textures, "element_fire", "icons/icon_fire.png");
	Add(textures, "element_ice", "icons/icon_ice.png");
	Add(textures, "element_wind", "icons/icon_wind.png");
	Add(textures, "element_light", "icons/icon_light.png");
	Add(textures, "element_dark", "icons/icon_dark.png");
	Add(textures, "element_earth", "icons/icon_earth.png");
	Add(textures, "element_any", "icons/icon_any_element.png");
	Add(textures, "element_spend", "icons/icon_spend_element.png");
	Add(textures, "brute", "icons/player_brute.png");
}

void Icons::Unload()
{
}

Texture* Icons::Get(const std::string& name)
{
	auto name_dest = std::string();
	name_dest.resize(name.size());
	std::transform(name.begin(), name.end(), name_dest.begin(), std::tolower);

	auto it = icons.find(name_dest);

	if(it == icons.end())
		return nullptr;

	auto pair = *it;
	return &pair.second;
}

Texture* Icons::GetPlayerCard()
{
	return &texturePlayerCard;
}

void Icons::Add(TextureService& textures, const std::string& name, const std::string& relativePath)
{
	icons[name] = textures.LoadTexture("textures/" + relativePath);
}
