#include"Level.h"
#include"pch/pch.h"

unsigned int PixelToValue(unsigned char r, unsigned char g, unsigned char b)
{
	return (r << 16) + (g << 8) + b;
}

Level::Level()
{
	pages = 0;
	page_x = 0;
	page_y = 0;
	coinsTotal = 0;

	startX = 3;
	startY = 32;
}


Level::~Level()
{
}

void Level::resetAllItems()
{
	for(unsigned int y = 0; y < 2; ++y)
	{
		for(unsigned int x = 0; x < 3; ++x)
		{
			Page* p = pages[y][x];
			for(unsigned int i = 0; i < p->items.size(); ++i)
				p->items[i]->bActive = true;
		}
	}
}

bool Level::SetPage(int x, int y, Entity& player)
{
	if(x < 0 || y < 0)
		return false;

	if(x >= static_cast<int>(pageWidth) || y >= static_cast<int>(pageHeight))
		return false;

	page_x = x;
	page_y = y;
	curPage = pages[y][x];

	player.pageX = page_x;
	player.pageY = page_y;

	return true;
}


void Level::SpawnEntity(int gridX, int gridY, Entity& ent)
{
	if(gridX < 0 || gridY < 0)
		return;

	if(gridX > 65 || gridY > 37)
		return;

	float posX = float(gridX) * stepSizeX + halfStepX;
	float posY = float(gridY) * stepSizeY;

	ent.pos.x = posX;
	ent.pos.y = posY;

	ent.velocity.x = 0;
	ent.velocity.y = 0;
}


void Level::ProcessSprites()
{
	if(!sprites)
	{
		delete[]sprites;
		sprites = 0;
	}

	sprites = vnew Sprite[8 * 8];

	float stepSize = 16.0f / sprite0.width;

	for(unsigned int y = 0; y < 8; ++y)
	{
		for(unsigned int x = 0; x < 8; ++x)
		{
			int spriteIndex = y * 8 + x;

			sprites[spriteIndex].x = x;
			sprites[spriteIndex].y = y;
			sprites[spriteIndex].width = 1;
			sprites[spriteIndex].height = 1;

			sprites[spriteIndex].texcoords[0].x = sprites[spriteIndex].x * stepSize;
			sprites[spriteIndex].texcoords[0].y = sprites[spriteIndex].y * stepSize;

			sprites[spriteIndex].texcoords[1].x = (sprites[spriteIndex].x + sprites[spriteIndex].width) * stepSize;
			sprites[spriteIndex].texcoords[1].y = sprites[spriteIndex].y * stepSize;

			sprites[spriteIndex].texcoords[2].x = (sprites[spriteIndex].x + sprites[spriteIndex].width) * stepSize;
			sprites[spriteIndex].texcoords[2].y = (sprites[spriteIndex].y + sprites[spriteIndex].height) * stepSize;

			sprites[spriteIndex].texcoords[3].x = sprites[spriteIndex].x * stepSize;
			sprites[spriteIndex].texcoords[3].y = (sprites[spriteIndex].y + sprites[spriteIndex].height) * stepSize;
		}
	}
}

Sprite* Level::getSprite(unsigned int x, unsigned int y)
{
	if(!sprites)
		return 0;

	if(x >= 8 || y >= 8)
		return 0;

	return &sprites[x + y * 8];
}

const unsigned int iSpike = PixelToValue(0, 255, 0);
const unsigned int iSpikeDOWN = PixelToValue(0, 255, 1);
const unsigned int iSpikeRIGHT = PixelToValue(0, 255, 2);
const unsigned int iSpikeLEFT = PixelToValue(0, 255, 3);

const unsigned int iStartPoint = PixelToValue(128, 0, 0);
const unsigned int iAntiGravity = PixelToValue(255, 128, 0);
const unsigned int iLadder = PixelToValue(0, 0, 255);
const unsigned int iCoin = PixelToValue(5, 255, 246);
const unsigned int iDoubleJumpUpgrade = PixelToValue(255, 0, 110);
const unsigned int iCheckPoint = PixelToValue(255, 0, 255);
const unsigned int iInformation = PixelToValue(255, 0, 0);
const unsigned int iAchievement = PixelToValue(255, 215, 0);
const unsigned int iWallJumpUpgrade = PixelToValue(255, 216, 0);
const unsigned int iWallTrap = PixelToValue(255, 255, 254);
const unsigned int iWall = PixelToValue(255, 255, 255);

//does not make anything bigger than 1x1 geometry
void Level::ProcessPixel(const int x, const int y, const unsigned int value, Page* page)
{
	unsigned int colMapIndex = x + y * page->width;
	unsigned int colMapValue = 0;
	unsigned int spriteX = 0;
	unsigned int spriteY = 0;
	unsigned int spriteW = 1;
	unsigned int spriteH = 1;

	//part of the level, or an interactable object?
	bool bGeometry = false;

	unsigned int itemType = 0;

	Item* item = 0;




	//Void
	if(value == 0)
	{
		return;
	}
	else if(value == iStartPoint)
	{
		startX = (page->x << 16) + x;
		startY = (page->y << 16) + y;
		return;
	}
	else if(value == iAntiGravity)
	{
		colMapValue = 301;
		spriteX = 6;
		spriteY = 2;
		bGeometry = true;
	}
	else if(value == 0xFF6100)
	{
		colMapValue = 137;
		spriteX = 7;
		spriteY = 0;
		bGeometry = true;
	}
	else if(value == 0xFF9EEF)
	{
		page->colMap[colMapIndex] = 136;
		return;
	}
	else if(value == 0xFF9EEE)
	{
		colMapValue = 136;
		spriteX = 6;
		spriteY = 0;
		bGeometry = true;
	}
	else if(value >= iSpike && value <= iSpike + 3)
	{
		int offset = value - iSpike;

		spriteX = offset;
		spriteY = 2;
		colMapValue = 101;
	}
	else if(iWallJumpUpgrade == value)
	{
		spriteX = 1;
		spriteY = 1;
		colMapValue = 1300;
	}
	else if(iDoubleJumpUpgrade == value)
	{
		spriteX = 0;
		spriteY = 1;
		colMapValue = 1301;
	}
	else if(iCoin == value)
	{
		spriteX = 3;
		spriteY = 0;
		colMapValue = 55;
		coinsTotal++;
	}
	else if(iCheckPoint == value)
	{
		item = vnew Spawner;

		Spawner* s = (Spawner*)item;
		s->pageX = page->x;
		s->pageY = page->y;
		s->states[0] = getSprite(5, 0);
		s->states[1] = getSprite(4, 0);

		spriteX = 5;
		spriteY = 0;
		colMapValue = 1299;
	}
	else if(value >= iInformation && value <= (iInformation + 254))
	{
		spriteX = 3;
		spriteY = 1;
		colMapValue = 1500 + (value - iInformation);
		bGeometry = true;
	}
	else if(iAchievement == value)
	{
		spriteX = 4;
		spriteY = 1;
		colMapValue = 1701;
	}
	else if(iLadder == value)
	{
		spriteX = 2;
		spriteY = 0;
		colMapValue = 2;

		bGeometry = true;
	}
	else if(iWall == value)
	{
		spriteX = 1;
		spriteY = 0;
		bGeometry = true;
		colMapValue = 1;
	}
	else if(iWallTrap == value)
	{
		spriteX = 6;
		spriteY = 1;
		bGeometry = false;
		colMapValue = 3;
	}

	page->colMap[colMapIndex] = colMapValue;

	if(bGeometry)
	{
		Sprite* s = getSprite(spriteX, spriteY);

		for(int i = 0; i < 4; ++i)
			page->texcoords.push_back(s->texcoords[i]);

		page->verts.push_back(glm::vec2(x, y));
		page->verts.push_back(glm::vec2(x + 1, y));
		page->verts.push_back(glm::vec2(x + 1, y + 1));
		page->verts.push_back(glm::vec2(x, y + 1));
	}
	else
	{
		if(!item)
			item = vnew Item;

		item->ID = (x + y * page->width) + ((page->x + page->y * 8) << 16);
		item->x = x;
		item->y = y;

		item->verts[0] = glm::vec2(x, y);
		item->verts[1] = glm::vec2(x + 1, y);
		item->verts[2] = glm::vec2(x + 1, y + 1);
		item->verts[3] = glm::vec2(x, y + 1);

		item->sprite = getSprite(spriteX, spriteY);

		if(value != iCheckPoint)
			page->items.push_back(item);
		else
			spawners.push_back((Spawner*)item);
	}
}

#include"textures_pngloader.h"

Page* Level::LoadPage(unsigned int pageX, unsigned int pageY, const ImageData& texData)
{
	Page* p = vnew Page;

	memset(p->colMap, 0, sizeof(int) * 65 * 37);
	p->x = pageX;
	p->y = pageY;
	p->width = 65;
	p->height = 37;

	unsigned int imageX = pageX * p->width;
	unsigned int imageY = pageY * p->height;

	for(unsigned int y = 0; y < p->height; ++y)
	{
		for(unsigned int x = 0; x < p->width; ++x)
		{
			int index = (x + imageX) + (y + imageY) * texData.width;
			index *= texData.channels;

			unsigned char* c = &texData.data[index];
			ProcessPixel(x, y, PixelToValue(c[0], c[1], c[2]), p);
		}
	}


	p->bBuffers = false;
	if(p->verts.size())
	{
		glGenBuffers(1, &p->bufID[0]);
		glBindBuffer(GL_ARRAY_BUFFER, p->bufID[0]);
		glBufferData(GL_ARRAY_BUFFER, p->verts.size() * sizeof(glm::vec2), &p->verts[0].x, GL_STATIC_DRAW);

		glGenBuffers(1, &p->bufID[1]);
		glBindBuffer(GL_ARRAY_BUFFER, p->bufID[1]);
		glBufferData(GL_ARRAY_BUFFER, p->texcoords.size() * sizeof(glm::vec2), &p->texcoords[0].x, GL_STATIC_DRAW);
		p->bBuffers = true;
	}

	return p;
}

void Level::LoadMap(const char* file)
{
	PNGLoader loader;
	auto status = loader.Read(file);

	if(status != ImageLoaderStatus::LOADER_SUCCESS) {
		throw "Failed to load map";
	}

	auto mapData = loader.Get();

	pageWidth = mapData.width / 65;
	pageHeight = mapData.height / 37;


	pages = vnew std::vector<Page*>[pageHeight];

	for(unsigned int y = 0; y < pageHeight; ++y)
	{
		for(unsigned int x = 0; x < pageWidth; ++x)
		{
			Page* p = LoadPage(x, y, mapData);
			pages[y].push_back(p);
		}
	}

	loader.Free();

	page_x = 0;
	page_y = 0;
	curPage = pages[page_y][page_x];
}


void Level::Destroy()
{
	for(unsigned int y = 0; y < pageHeight; ++y)
	{
		for(unsigned int x = 0; x < pageWidth; ++x)
		{
			std::vector<Item*>& itemVec = pages[y][x]->items;

			for(unsigned int j = 0; j < itemVec.size(); ++j)
				delete itemVec[j];
			itemVec.clear();

			if(pages[y][x]->bBuffers)
			{
				glDeleteBuffers(1, &pages[y][x]->bufID[0]);
				glDeleteBuffers(1, &pages[y][x]->bufID[1]);
			}

			pages[y][x]->verts.clear();
			pages[y][x]->texcoords.clear();
			delete pages[y][x];
		}
	}

	delete[]pages;

	for(unsigned int j = 0; j < spawners.size(); ++j)
		delete spawners[j];

	//sprite0.unload();
	delete[]sprites;
}

void Level::remItem(int x, int y)
{
	for(unsigned int i = 0; i < curPage->items.size(); ++i)
	{
		Item& item = *curPage->items[i];
		if(item.x == x && item.y == y)
		{
			item.bActive = false;
			//curPage->items.erase( curPage->items.begin() + i );
			return;
		}
	}
}

Item* Level::getItem(int x, int y)
{
	for(unsigned int i = 0; i < curPage->items.size(); ++i)
	{
		Item& item = *curPage->items[i];
		if(item.x == x && item.y == y)
		{
			return &item;
		}
	}

	return 0;
}

Item* Level::getItem(unsigned int ID)
{
	for(unsigned int y = 0; y < 2; ++y)
	{
		for(unsigned int x = 0; x < 3; ++x)
		{
			Page* p = pages[y][x];

			for(unsigned int i = 0; i < p->items.size(); ++i)
			{
				if(p->items[i]->ID == ID)
					return p->items[i];
			}
		}
	}

	return 0;
}

Spawner* Level::getSpawner(int x, int y)
{
	for(unsigned int i = 0; i < spawners.size(); ++i)
	{
		Spawner& spawner = *spawners[i];
		if(spawner.x == x && spawner.y == y)
		{
			return &spawner;
		}
	}

	return 0;
}

Spawner* Level::getSpawner(int ID)
{
	for(unsigned int i = 0; i < spawners.size(); ++i)
	{
		Spawner& spawner = *spawners[i];
		if(spawner.ID == ID)
		{
			return &spawner;
		}
	}

	return 0;
}


void Level::Update()
{
}


void Level::PushRenderState()
{
	glPushMatrix();
	glScalef(stepSizeX, stepSizeY, 1);
	glColor3f(1, 1, 1);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, sprite0.textureId);

	glEnable(GL_BLEND);
}

void Level::PopRenderState()
{
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_TEXTURE_2D);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glPopMatrix();
}

void Level::Render()
{
}

void Level::RenderLevel()
{
	glBindBuffer(GL_ARRAY_BUFFER, curPage->bufID[0]);
	glVertexPointer(2, GL_FLOAT, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, curPage->bufID[1]);
	glTexCoordPointer(2, GL_FLOAT, 0, 0);

	glDrawArrays(GL_QUADS, 0, static_cast<GLsizei>(curPage->verts.size()));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Level::RenderEntities(unsigned int spawnerID)
{
	for(unsigned int i = 0; i < curPage->items.size(); ++i)
	{
		Item& item = *curPage->items[i];

		if(item.bActive)
			item.Render();
	}


	//Spawners
	for(unsigned int i = 0; i < spawners.size(); ++i)
	{
		Spawner& spawner = *spawners[i];

		if(spawner.pageX != page_x || spawner.pageY != page_y)
			continue;


		if(spawner.ID == spawnerID)
			spawner.sprite = spawner.states[1];
		else spawner.sprite = spawner.states[0];

		spawner.Render();
	}
}


float Level::LineGridCheck(const glm::vec2& from, const glm::vec2& to)
{
	glm::vec2 dir = to - from;
	glm::vec2 ndir = normalize(dir);
	int signX = ndir.x > 0 ? 1 : -1;
	int signY = ndir.y > 0 ? 1 : -1;

	glm::vec2 start = glm::vec2(floor(from.x / stepSizeX) * stepSizeX, floor(from.y / stepSizeY) * stepSizeY);
	glm::vec2 end = glm::vec2(floor(to.x / stepSizeX) * stepSizeX, floor(to.y / stepSizeY) * stepSizeY);
	int startX = (int)(from.x / stepSizeX);
	int startY = (int)(from.y / stepSizeY);
	int endX = (int)(to.x / stepSizeX);
	int endY = (int)(to.y / stepSizeY);


	float tDeltaX = stepSizeX / fabs(ndir.x);
	float tDeltaY = stepSizeY / fabs(ndir.y);

	float fDeltaX = stepSizeX / ndir.x;
	float fDeltaY = stepSizeX / ndir.y;

	float tMaxX = 0.0f;
	float tMaxY = 0.0f;

	if(ndir.x < 0.0f)
		tMaxX = (start.x - from.x) / ndir.x;
	else if(ndir.x > 0.0f)
		tMaxX = (start.x + stepSizeX - from.x) / ndir.x;
	else tMaxX = 99999.0f;

	if(ndir.y < 0.0f)
		tMaxY = (start.y - from.y) / ndir.y;
	else if(ndir.y > 0.0f)
		tMaxY = (start.y + stepSizeY - from.y) / ndir.y;
	else tMaxY = 99999.0f;

	int x = startX;
	int y = startY;

	unsigned int ptX = startX, ptY = startY;

	bool bCollide = false;
	int tempX = x;
	int tempY = y;

	int colX = -1;
	int colY = -1;

	while(true)
	{
		if(tMaxX < tMaxY)
		{
			tMaxX += tDeltaX;
			x += 1 * signX;
		}
		else
		{
			tMaxY += tDeltaY;
			y += 1 * signY;
		}

		bool rX = false;
		bool rY = false;

		if(!rX)
		{
			if(signX > 0)
			{
				if(x >= endX)
					rX = true;
			}
			else if(x <= endX)
				rX = true;
		}

		if(!rY)
		{
			if(signY > 0)
			{
				if(y >= endY)
					rY = true;
			}
			else if(y <= endY)
				rY = true;
		}

		unsigned int index = x + y * curPage->width;
		if(index >= 0 && index < curPage->width * curPage->height)
		{
			if(curPage->colMap[index] == 1)
			{
				if(!bCollide)
				{
					colX = x;
					colY = y;
					tempX = x;
					tempY = y;
				}

				bCollide = true;
				//break;
			}
		}

		if(!bCollide)
		{
			ptX = x;
			ptY = y;
		}

		if(rX && rY)
			break;
	}

	x = tempX;
	y = tempY;

	glm::vec2 isect(0, 0);

	if(bCollide)
	{
		unsigned int tX = colX - ptX;
		unsigned int tY = colY - ptY;

		x = colX;
		y = colY;
		//(0,1)
		glm::vec2 p3(x * stepSizeX, y * stepSizeY);
		glm::vec2 p4((x + 1) * stepSizeX, y * stepSizeY);
		float f0 = ((p4.x - p3.x) * (from.y - p3.y) - (p4.y - p3.y) * (from.x - p3.x)) / ((p4.y - p3.y) * (to.x - from.x) - (p4.x - p3.x) * (to.y - from.y));

		//(1,0)
		p3 = glm::vec2(x * stepSizeX, y * stepSizeY);
		p4 = glm::vec2(x * stepSizeX, (y + 1) * stepSizeY);
		float f1 = LineIntersection(from, to, p3, p4);

		//0,-1
		p3 = glm::vec2(x * stepSizeX, (y + 1) * stepSizeY);
		p4 = glm::vec2((x + 1) * stepSizeX, (y + 1) * stepSizeY);
		float f2 = LineIntersection(from, to, p3, p4);

		//-1,0
		p3 = glm::vec2((x + 1) * stepSizeX, y * stepSizeY);
		p4 = glm::vec2((x + 1) * stepSizeX, (y + 1) * stepSizeY);
		float f3 = LineIntersection(from, to, p3, p4);

		float fraction = f0;

		if(tX == 1 && tY == 0) fraction = f1;
		if(tX == 0 && tY == -1) fraction = f2;
		if(tX == -1 && tY == 0) fraction = f3;

		/*
		p3 = glm::vec2( x*stepSizeX, y*stepSizeY );
		p4 = glm::vec2((x+tX)*stepSizeX, (y+tY)*stepSizeY );


		float f4 = LineIntersection(from, to, p3, p4);
		*/

		if(fraction < 0.0f)
			fraction = 0.0f;

		if(fraction > 1.0f)
			fraction = 1.0f;

		return fraction;
	}

	return 1.0f;
}


float Level::BoxGridCheck(const glm::vec2& from, const glm::vec2& to, const glm::vec2& size)
{
	glm::vec2 vCorner = glm::vec2(size.x * halfStepX, size.y * halfStepY);

	glm::vec2 p0, p1;
	glm::vec2 dir = to - from;

	p0 = from + vCorner;
	p1 = to + vCorner;
	float fraction = LineGridCheck(p0, p1);

	vCorner = glm::vec2(size.x * halfStepX, -size.y * halfStepY);
	p0 = from + vCorner;
	p1 = to + vCorner;
	float fraction1 = LineGridCheck(p0, p1);

	vCorner = glm::vec2(-size.x * halfStepX, -size.y * halfStepY);
	p0 = from + vCorner;
	p1 = to + vCorner;
	float fraction2 = LineGridCheck(p0, p1);

	vCorner = glm::vec2(-size.x * halfStepX, size.y * halfStepY);
	p0 = from + vCorner;
	p1 = to + vCorner;
	float fraction3 = LineGridCheck(p0, p1);

	if(fraction < 0.001f)
		fraction = 0.0f;

	if(fraction1 < fraction && fraction1 >= 0.001f)
		fraction = fraction1;

	if(fraction2 < fraction && fraction2 >= 0.001f)
		fraction = fraction2;

	if(fraction3 < fraction && fraction3 >= 0.001f)
		fraction = fraction3;

	return fraction;
}



int Level::WallCheck(glm::vec2& pos, glm::vec2& size)
{
	//RIGHT

	//top
	glm::vec2 check = glm::vec2(size.x * halfStepX, size.y * halfStepY);
	if(LineGridCheck(pos + check, pos + check + glm::vec2(0.1f, 0)) < 1.0f)
		return 1;

	//bottom
	check.y *= -1;
	if(LineGridCheck(pos + check, pos + check + glm::vec2(0.1f, 0)) < 1.0f)
		return 1;


	//LEFT
	//bottom
	check.x *= -1;

	if(LineGridCheck(pos + check, pos + check - glm::vec2(0.1f, 0)) < 1.0f)
		return -1;

	//top
	check.y *= -1;
	if(LineGridCheck(pos + check, pos + check - glm::vec2(0.1f, 0)) < 1.0f)
		return -1;


	return 0;
}

//Possibly flawed
//- need to know the direction of the velocity
int Level::HorizontalCheck(const glm::vec2& pos, const glm::vec2& size)
{
	//TOP
	//right
	glm::vec2 check = glm::vec2(size.x * halfStepX, size.y * halfStepY);
	if(LineGridCheck(pos + check, pos + check + glm::vec2(0, 0.1f)) < 1.0f)
		return 1;

	//left
	check.x *= -1;
	if(LineGridCheck(pos + check, pos + check + glm::vec2(0, 0.1f)) < 1.0f)
		return 1;

	//Bottom
	//left
	check.y *= -1;

	if(LineGridCheck(pos + check, pos + check - glm::vec2(0, 0.1f)) < 1.0f)
		return -1;

	//right
	check.y *= -1;
	if(LineGridCheck(pos + check, pos + check - glm::vec2(0, 0.1f)) < 1.0f)
		return -1;


	return 0;
}


float Level::LineIntersection(const glm::vec2& p0, const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3)
{
	float fraction = ((p3.x - p2.x) * (p0.y - p2.y) - (p3.y - p2.y) * (p0.x - p2.x)) / ((p3.y - p2.y) * (p1.x - p0.x) - (p3.x - p2.x) * (p1.y - p0.y));
	return fraction;
}


unsigned int Level::CalculateMemoryUsed()
{
	unsigned int memAccum = 0;

	memAccum += 37 * 65 * sizeof(unsigned int);
	memAccum += static_cast<unsigned int>(pages[page_y][page_x]->verts.size()) * sizeof(glm::vec2);
	memAccum += static_cast<unsigned int>(pages[page_y][page_x]->texcoords.size()) * sizeof(glm::vec2);
	memAccum += static_cast<unsigned int>(pages[page_y][page_x]->items.size()) * sizeof(Item) + sizeof(Sprite);

	return memAccum;
}