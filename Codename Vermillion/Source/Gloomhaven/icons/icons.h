#pragma once

#include<string>
#include<map>

class Texture;
class TextureService;

class Icons {
public:

	static void Load(TextureService& textures);
	static void Unload();
	static Texture* Get(const std::string& name);
	static Texture* GetPlayerCard();

private:
	static std::map<std::string, Texture> icons;
	static Texture texturePlayerCard;
};