#pragma once

class Level;

class Spawner 
{
public:
	Spawner(Level&level);
	~Spawner();

	void SpawnPlayer(glm::ivec3 location);
	void SpawnMonster(glm::ivec3 location, bool elite);

private:
	Level& level;
	int spawnerIdGenerator;
};