#pragma once

//#include"SoundManager.h"
#include"Entity.h"
#include"Level.h"


class EntityManager
{
public:
	EntityManager();
	~EntityManager();

	void SpawnPlayer(Entity& plr, int spawnID = -1);

	void Update(float fDelta, Entity& plr);
	void UpdatePlayer(float fDelta, sInputFlags& input, Entity& plr);

	void Render();

	int InfoID;
	bool bInfo;
	bool bGodMode;
	Entity player;
	Entity ghost;

	std::vector<Item*> pickups;
	std::vector<Item*> prevPickups;
	unsigned int prevCoins;

	Level* level;
	//SoundManager* sndMgr;

	std::vector<glm::vec2> spaceVec;
	std::vector<glm::vec2> failVec;

	unsigned int spawnerCounter; //how many times did the player activate a spawner
	unsigned int killCounter;
	unsigned int coinCounter;

	bool bGameStarted;
	bool bGameEnded;

private:

};