#pragma once


#include"../Framework/activity.h"

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
	std::vector<UILayer*> layers;
};