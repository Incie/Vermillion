#pragma once

#include"Entity.h"
#include"glm/glm.hpp"
#include"services.h"
//#include"Texture.h"
#include<vector>
#include"GL/glew.h"

class ImageData;

struct Sprite
{
	glm::vec2 texcoords[4];

	int x;
	int y;
	int width;
	int height;
};


struct Item
{
	Item() :ID(0), bActive(true), x(0), y(0), sprite(0) {}
	unsigned int ID;

	bool bActive;
	unsigned int x, y;
	glm::vec2 verts[4];

	Sprite* sprite;

	void Render()
	{
		if(!sprite)
			return;

		glVertexPointer(2, GL_FLOAT, 0, verts);
		glTexCoordPointer(2, GL_FLOAT, 0, sprite->texcoords);

		glDrawArrays(GL_QUADS, 0, 4);
	}
};

struct Spawner : public Item
{
	Sprite* states[2]; //On or Off
	int pageX, pageY;
	//unsigned int playerFlags;
};

struct Page
{
	int colMap[65 * 37];
	unsigned int x, y;
	unsigned int width, height;

	std::vector<glm::vec2> verts;
	std::vector<glm::vec2> texcoords;
	std::vector<Item*> items;

	unsigned int bufID[2];
	bool bBuffers;
};


//Collisionmap Value
enum
{
	CVAL_COIN,
	CVAL_DBLJUMP,
	CVAL_INFO
};


class Level
{
public:
	Level();
	~Level();

	void remItem(int x, int y);
	Item* getItem(int x, int y);
	Item* getItem(unsigned int ID);
	Spawner* getSpawner(int x, int y);
	Spawner* getSpawner(int ID);
	void resetAllItems();
	//Item *getItem(unsigned int ID)

	void LoadMap(const char* file);
	void Destroy();


	void Update();
	/***/
	void PushRenderState();
	void PopRenderState();
	void Render();
	void RenderLevel();
	void RenderEntities(unsigned int spawnerID);
	/***/

	float BoxGridCheck(const glm::vec2& from, const glm::vec2& to, const glm::vec2& size);
	float LineGridCheck(const glm::vec2& from, const glm::vec2& to);
	int WallCheck(glm::vec2& pos, glm::vec2& size);
	int HorizontalCheck(const glm::vec2& pos, const glm::vec2& size);
	float LineIntersection(const glm::vec2& p0, const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3);

	int toGridX(float val) { return (int)(val / stepSizeX); }
	int toGridY(float val) { return (int)(val / stepSizeY); }
	int toGridX(glm::vec2& v) { return (int)(v.x / stepSizeX); }
	int toGridY(glm::vec2& v) { return (int)(v.y / stepSizeY); }

	bool SetPage(int x, int y, Entity& player);
	void SpawnEntity(int gridX, int gridY, Entity& ent);


	unsigned int CalculateMemoryUsed();

	//private:
	Page* LoadPage(unsigned int pageX, unsigned int pageY, const ImageData& texData);
	unsigned int pageWidth;
	unsigned int pageHeight;

	char Filename[128]; //unused?

	std::vector<Page*>* pages;
	Page* curPage;
	unsigned int page_x, page_y;

	float stepSizeX, stepSizeY;
	float halfStepX, halfStepY;

	void ProcessPixel(const int x, const int y, const unsigned int value, Page* page);
	void ProcessSprites();
	Sprite* getSprite(unsigned int x, unsigned  int y);

	std::vector<Spawner*> spawners;
	Texture sprite0;
	Sprite* sprites; //only one sheet so far, sprite0.png

	unsigned int coinsTotal;

	int startX;
	int startY;
};