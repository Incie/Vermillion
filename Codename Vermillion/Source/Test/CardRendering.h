#pragma once

#include"../Framework/activity.h"
#include"../Gloomhaven/cards/CardGenerator.h"
#include"../textures.h"
#include"glm/glm.hpp"
#include<vector>

class PlayerCard;
class UILayer;

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
	CardGenerator cardGenerator;
	std::vector<UILayer*> layers;
	std::vector<PlayerCard> cards;

	Texture card0;
	Texture card1;
	glm::vec2 p;
};