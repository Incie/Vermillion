#pragma once

#include"activity.h"

class ActivityMainMenu : public Activity
{
public:
	ActivityMainMenu() {}
	~ActivityMainMenu() {}

	void Initialize();
	void Deinitialize();

	void Update(float fDelta);
	void Render();

	void HandlePacket(void* p) {}

	void StartGame(int type);
private:
};