#pragma once


#include"glm/glm.hpp"

enum
{
	ENT_GROUNDED = (1 << 0),
	ENT_GRAVITY = (1 << 1),
	ENT_JUMP = (1 << 2), //unused
	ENT_MOVERIGHT = (1 << 3), //unused
	ENT_DBLJUMP = (1 << 4),
	ENT_DBLJUMPUPGRADE = (1 << 5), //Powerup-flag
	ENT_WALLJUMPUPGRADE = (1 << 6),//Powerup-flag
	ENT_LADDERING = (1 << 7),
	ENT_SLIDING = (1 << 8),
	ENT_ENDLEVEL = (1 << 9),
	ENT_INJUMP = (1 << 10)
};

class Entity
{
public:
	Entity() :Status(0), spawnerID(0), bGhost(false), pageX(0), pageY(0), spawnFlags(0), bLeft(false), frameTimer(0.0f), frame(0) {}
	~Entity() {}

	void Update(float fDelta, float stepSizeX);
	void Render(float halfSizeX, float halfSizeY);

	bool checkFlag(unsigned int mask) { return ((Status & mask) != 0); }
	void flag(unsigned int mask) { Status |= mask; }
	/**   (0110 &= 0100 ^ -1) == 0010
		->  0100 ^ -1 = 1011
		->         -1 = 1111    **/
	void unflag(unsigned int mask) { Status &= mask ^ -1; }

	glm::vec2 pos;
	glm::vec2 size;

	glm::vec2 velocity;
	glm::vec2 maxvelocity;
	glm::vec2 acceleration;
	glm::vec2 friction; //stopping speed
	glm::vec2 walljump;

	glm::vec2 tryMove;

	unsigned int spawnerID;
	unsigned int spawnFlags;
	unsigned int Status;

	float fTimeElapsed;
	bool bFinished;

	bool bLeft;
	float frameTimer;
	int frame;

	int pageX;
	int pageY;
	bool bGhost;
private:

};


struct sInputFlags
{
	int moveVertical;
	int moveHorizontal;
	int jump;

	int showjumps;
	int click[3];
	int help;

	int suicide;

	glm::vec2 vMouse;
};