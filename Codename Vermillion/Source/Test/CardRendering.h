#pragma once

#include"../Framework/activity.h"
#include"../Gloomhaven/cards/CardGenerator.h"
#include"../textures.h"
#include"../Gloomhaven/level/Level.h"
#include"glm/glm.hpp"
#include<vector>

class PlayerCard;
class UIView;

class CardRendering : public Activity {
public:
	CardRendering();
	~CardRendering();

	void Initialize();
	void Deinitialize();

	void Resize();

	void Update(double delta);
	void Render();

private:
	Level level;
	CardGenerator cardGenerator;
	std::vector<UIView*> layers;
	std::vector<PlayerCard> cards;

	Texture card0;
	Texture card1;
	glm::vec2 p;
};