#pragma once

#include"activity.h"
#include"glm/glm.hpp"
#include"EntityManager.h"
#include"Level.h"

class ActivitySideScroller : public Activity
{
public:
	ActivitySideScroller() {}
	~ActivitySideScroller() {}

	void Initialize();
	void Deinitialize();

	void Update(float fDelta);
	void Render();

	char name[32];
	glm::vec3 color;

	void NewGame();
	bool LoadGame();
	void StartReplay(char* file);
	void StartRace();

	//SoundManager sndMgr;
	bool bRace;
	bool bPlayBack;
	float fTimeStamp;
	EntityManager entityManager;
	Level lvl;
private:
};