#pragma once


#include"../Framework/activity.h"

class CardRendering : public Activity {
public:
	CardRendering();
	~CardRendering();

	void Initialize();
	void Deinitialize();

	void Update(double delta);
	void Render();

private:

};