#include"pch/pch.h"
#include"ActivitySideScroller.h"
#include"Level.h"
#include"EntityManager.h"
#include<vector>

bool bShowJumps = false;
bool showDEBUG = false;

Texture texRun[6];
Texture texClimb[6];
Texture texIdle;
Texture texJump;
Texture texFall;
Texture texSlide;
Texture texWallSlide;
Texture texDoubleJump;

sInputFlags inputFlags = {0};
//sInputFlags gInput = {0};

const char* sInfo[] = {
	"Careful now, this looks dangerous",
	"This is a checkpoint, touch it to activate",
	"It looks like a ladder.",
	"You can get a boost from ladders... somehow.",
	"Caution: slippery surfaces ahead!",
	"I feel more energetic",
	"Teehee.",
	"Have you lost mass?",
	"wat.",
	"<--- That way.",
	"Fin. (for now, so grats!)"
};


void ActivitySideScroller::NewGame()
{
	entityManager.bGameStarted = true;
	entityManager.bGameEnded = false;

	lvl.resetAllItems();

	entityManager.SpawnPlayer(entityManager.player, -1);

	entityManager.killCounter = 0;
	entityManager.coinCounter = 0;
	entityManager.prevCoins = 0;
	entityManager.spawnerCounter = 0;

	entityManager.pickups.clear();
	entityManager.prevPickups.clear();
	entityManager.spaceVec.clear();
	entityManager.failVec.clear();

	bRace = false;
	bPlayBack = false;
	fTimeStamp = 0.0f;
}

bool ActivitySideScroller::LoadGame()
{
	int spawnerID = -1;
	int killCounter = 0;
	int coinCounter = 0;
	int status = 0;
	float timeElapsed = 0.0f;

	std::vector<unsigned int> pickups;

	FILE* pFile = 0;
	fopen_s(&pFile, "data/auto.sav", "rb");

	if(!pFile) return false;

	fread(&spawnerID, sizeof(int), 1, pFile);
	fread(&status, sizeof(int), 1, pFile);
	fread(&killCounter, sizeof(int), 1, pFile);
	fread(&coinCounter, sizeof(int), 1, pFile);
	fread(&timeElapsed, sizeof(float), 1, pFile);

	while(!feof(pFile))
	{
		unsigned int ID = -1;
		fread(&ID, sizeof(unsigned int), 1, pFile);

		Item* i = lvl.getItem(ID);

		if(i)
		{
			entityManager.pickups.push_back(i);
			i->bActive = false;
		}
	}

	fclose(pFile);

	Spawner* s = lvl.getSpawner(spawnerID);
	if(s)
	{
		entityManager.player.spawnFlags = status;
	}

	entityManager.SpawnPlayer(entityManager.player, spawnerID);
	entityManager.killCounter = killCounter;
	entityManager.coinCounter = coinCounter;
	entityManager.prevCoins = coinCounter;
	entityManager.player.fTimeElapsed = timeElapsed;
	entityManager.player.flag(ENT_GRAVITY);
	if(status & ENT_DBLJUMPUPGRADE) entityManager.player.flag(ENT_DBLJUMPUPGRADE);
	if(status & ENT_WALLJUMPUPGRADE)entityManager.player.flag(ENT_WALLJUMPUPGRADE);

	remove("data/auto.sav");

	return true;
}

void ActivitySideScroller::StartReplay(char* file)
{
	NewGame();
	//gGhost.LoadGhost(file);
	bPlayBack = true;
}


//Ghost g[25]; //list files func is in an euler project (3xx?)
Entity gp[25];
unsigned int numGhosts = 25;


void ActivitySideScroller::StartRace()
{
	NewGame();

	for(unsigned int i = 0; i < numGhosts; ++i)
	{
		//g[i].LoadGhost("data/dev1.ghost");
		//entMgr.SpawnPlayer(gp[i]);
		//gp[i].fTimeElapsed = -3.0f - float(i) * 0.5f;
		//gp[i].bGhost = true;
		//memset(&g[i].ghostInput, 0, sizeof(sInputFlags));
	}

	entityManager.SpawnPlayer(entityManager.ghost);
	entityManager.ghost.fTimeElapsed = -3.0f;
	entityManager.player.fTimeElapsed = -3.0f;

	bRace = true;
	fTimeStamp = -3.0f;
}


void Entity::Update(float fDelta, float stepSizeX)
{
	if(fabs(velocity.x) < 0.01f && fabs(velocity.y) < 0.01f)
	{
		frame = 3;
	}
	else
	{
		frameTimer += fDelta;

		float variableFPS = 30.0f;
		if(checkFlag(ENT_LADDERING))
			variableFPS = 10.0f;

		if(frameTimer >= 1 / (10.0f + variableFPS * (fabs(velocity.x) / (maxvelocity.x * stepSizeX))))
		{
			frameTimer = 0.0f;
			frame++;
			if(frame > 5)
				frame = 0;
		}
	}

	if(velocity.x < 0.0f)
		bLeft = true;
	if(velocity.x > 0.0f)
		bLeft = false;
}

void Entity::Render(float halfSizeX, float halfSizeY)
{
	glPushMatrix();

	glEnable(GL_TEXTURE_2D);
	float tcYMax = 0.8f;

	if(checkFlag(ENT_LADDERING))
	{
		glBindTexture(GL_TEXTURE_2D, texClimb[frame].textureId);
	}
	else if(checkFlag(ENT_GROUNDED) == false)
	{
		if(checkFlag(ENT_DBLJUMP))
			glBindTexture(GL_TEXTURE_2D, texDoubleJump.textureId);
		else
			glBindTexture(GL_TEXTURE_2D, texJump.textureId);

	}
	else if(fabs(velocity.x) < 0.01f)
		glBindTexture(GL_TEXTURE_2D, texIdle.textureId);
	else
	{
		if(checkFlag(ENT_SLIDING))
			glBindTexture(GL_TEXTURE_2D, texSlide.textureId);
		else
			glBindTexture(GL_TEXTURE_2D, texRun[frame].textureId);
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	float clampX = floor(pos.x + 0.5f);
	float clampY = (pos.y);

	glTranslatef(clampX, clampY, 0);

	float xVal = (size.x * halfSizeX);
	float yVal = (size.y * halfSizeY);

	//redundant, myes
	float xVal2 = -(size.x * halfSizeX);
	float yVal2 = (size.y * halfSizeY);

	if(bLeft && !checkFlag(ENT_LADDERING))
		glScalef(-1, 1, 1);



	if(checkFlag(ENT_LADDERING))
		tcYMax = 1.0f;

	glBegin(GL_QUADS);
	glTexCoord2f(0.2f, tcYMax); glVertex2f(xVal2, -yVal2);
	glTexCoord2f(0.8f, tcYMax); glVertex2f(xVal, -yVal2);
	glTexCoord2f(0.8f, 0); glVertex2f(xVal, yVal);
	glTexCoord2f(0.2f, 0); glVertex2f(xVal2, yVal);
	glEnd();


	if(showDEBUG)
	{
		glColor3f(1, 0, 0);
		glDisable(GL_TEXTURE_2D);
		glBegin(GL_LINE_LOOP);
		glVertex2f(xVal2, -yVal2);
		glVertex2f(xVal, -yVal2);
		glVertex2f(xVal, yVal);
		glVertex2f(xVal2, yVal);
		glEnd();
	}

	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

//#include"Framebuffer.h"
//#include"Shader.h"
//
//Shader blurV;
//Shader blurH;
//
//Framebuffer fbo0;
//Framebuffer fbo1;

void ActivitySideScroller::Initialize()
{
	//blurV.LoadShader("Shaders/blur.vert", "Shaders/blurV.frag");
	//blurH.LoadShader("Shaders/blur.vert", "Shaders/blurH.frag");

	//fbo0.Create(1050, 562);
	//fbo1.Create(1050, 562);


	//dbgText.CreateText("Arial", 22);
	//infoText.CreateText("Arial", 12);
	//chatText.CreateText("Arial", 18);
	//TextureManager TexMgr;

	auto& TexMgr = Services().Textures();

	texDoubleJump = TexMgr.LoadTexture(FilePath(Paths::Textures, "/Animations/player_dbljump0.png"));
	texWallSlide = TexMgr.LoadTexture(FilePath(Paths::Textures, "/Animations/wallslide0.png"));
	texFall = TexMgr.LoadTexture(  FilePath(Paths::Textures, "Animations/player_jump_.png"));
	texJump = TexMgr.LoadTexture(  FilePath(Paths::Textures, "Animations/player_jump_.png"));
	texIdle = TexMgr.LoadTexture(  FilePath(Paths::Textures, "Animations/player_idle_.png"));
	texSlide = TexMgr.LoadTexture( FilePath(Paths::Textures, "Animations/player_slide_.png"));
	texRun[0] = TexMgr.LoadTexture(FilePath(Paths::Textures, "Animations/test0.png"));
	texRun[1] = TexMgr.LoadTexture(FilePath(Paths::Textures, "Animations/test1.png"));
	texRun[2] = TexMgr.LoadTexture(FilePath(Paths::Textures, "Animations/test2.png"));
	texRun[3] = TexMgr.LoadTexture(FilePath(Paths::Textures, "Animations/test3.png"));
	texRun[4] = TexMgr.LoadTexture(FilePath(Paths::Textures, "Animations/test4.png"));
	texRun[5] = TexMgr.LoadTexture(FilePath(Paths::Textures, "Animations/test5.png"));

	texClimb[0] = TexMgr.LoadTexture(FilePath(Paths::Textures, "/Animations/player_climb0_.png"));
	texClimb[1] = TexMgr.LoadTexture(FilePath(Paths::Textures, "/Animations/player_climb1_.png"));
	texClimb[2] = TexMgr.LoadTexture(FilePath(Paths::Textures, "/Animations/player_climb2_.png"));
	texClimb[3] = TexMgr.LoadTexture(FilePath(Paths::Textures, "/Animations/player_climb3_.png"));
	texClimb[4] = TexMgr.LoadTexture(FilePath(Paths::Textures, "/Animations/player_climb4_.png"));
	texClimb[5] = TexMgr.LoadTexture(FilePath(Paths::Textures, "/Animations/player_climb5_.png"));


	lvl.sprite0 = TexMgr.LoadTexture(FilePath(Paths::Textures, "sprite1.png"));
	lvl.ProcessSprites();

	lvl.LoadMap("../Data/Textures/SS/Map3-2.png");
	//lvl.LoadMap("Data/Textures/SS/Multi0.png");
	//lvl.LoadMap("../Data/Textures/SS/Multi1.png");
	//lvl.LoadMap("../Data/Textures/SS/Map4.png");
	lvl.stepSizeX = (float)WindowState::Width() / 65.0f;
	lvl.stepSizeY = (float)WindowState::Height() / 37.0f;
	lvl.halfStepX = lvl.stepSizeX * 0.5f;
	lvl.halfStepY = lvl.stepSizeY * 0.5f;

	//scalars from original res to grid/s
	//16.153847, 15.189189


	//sndMgr.Initialize();
	//entMgr.sndMgr = &sndMgr;
	entityManager.level = &lvl;
	entityManager.SpawnPlayer(entityManager.player);

	bRace = false;
	bPlayBack = false;
	fTimeStamp = 0.0f;
}

void ActivitySideScroller::Deinitialize()
{
	lvl.Destroy();
}


void ActivitySideScroller::Update(float fDelta)
{
	auto& input = Services().Input();
	if(input.KeyOnce(VKey_ESCAPE))
	{
		if(!entityManager.bGameEnded)
		{
			FILE* pFile = 0;
			fopen_s(&pFile, "data/auto.sav", "wb");
			if(pFile)
			{
				fwrite(&entityManager.player.spawnerID, sizeof(unsigned int), 1, pFile);

				Spawner* s = lvl.getSpawner(entityManager.player.spawnerID);
				if(s)
					fwrite(&entityManager.player.spawnFlags, sizeof(unsigned int), 1, pFile);
				else //no spawner activated
				{
					unsigned int status = ENT_GRAVITY;
					fwrite(&status, sizeof(unsigned int), 1, pFile);
				}

				fwrite(&entityManager.killCounter, sizeof(unsigned int), 1, pFile);
				fwrite(&entityManager.prevCoins, sizeof(unsigned int), 1, pFile);
				fwrite(&entityManager.player.fTimeElapsed, sizeof(float), 1, pFile);

				for(unsigned int i = 0; i < entityManager.prevPickups.size(); ++i)
					fwrite(&entityManager.prevPickups[i]->ID, sizeof(unsigned int), 1, pFile);

				fclose(pFile);
			}
		}

		Finish();
		return;
	}

	entityManager.player.Update(fDelta, lvl.stepSizeX);

	if(bRace)
	{
		entityManager.ghost.Update(fDelta, lvl.stepSizeX);

		for(unsigned int i = 0; i < numGhosts; ++i)
			gp[i].Update(fDelta, lvl.stepSizeX);
	}

	lvl.Update();

	//TODO: Only perform if width or height has changed
	entityManager.player.pos.x /= lvl.stepSizeX;
	entityManager.player.pos.y /= lvl.stepSizeY;

	lvl.stepSizeX = (float)WindowState::Width() / 65.0f;
	lvl.stepSizeY = (float)WindowState::Height() / 37.0f;
	lvl.halfStepX = lvl.stepSizeX * 0.5f;
	lvl.halfStepY = lvl.stepSizeY * 0.5f;

	entityManager.player.pos.x *= lvl.stepSizeX;
	entityManager.player.pos.y *= lvl.stepSizeY;
	/*************/


	if(input.KeyOnce(VKey_F5)) showDEBUG = !showDEBUG;
	if(input.KeyOnce(VKey_F3)) bShowJumps = !bShowJumps;
	if(input.KeyOnce(VKey_BACK)) { entityManager.spaceVec.clear(); entityManager.failVec.clear(); }
	//if(input.KeyOnce('S')) sndMgr.bEnabled = !sndMgr.bEnabled;
	if(input.KeyDown(VKey_LBUTTON)) inputFlags.click[0] = 1;

	fTimeStamp += fDelta;

	if(!bPlayBack)
	{
		memset(&inputFlags, 0, sizeof(sInputFlags));

		//if(!input.kbHooked)
		{
			if(input.KeyDown(VKey_RIGHT) || input.KeyDown('D')) inputFlags.moveHorizontal += 1;
			if(input.KeyDown(VKey_LEFT) || input.KeyDown('A'))	inputFlags.moveHorizontal -= 1;
			if(input.KeyDown(VKey_UP) || input.KeyDown('W'))	inputFlags.moveVertical += 1;
			if(input.KeyDown(VKey_DOWN) || input.KeyDown('S'))	inputFlags.moveVertical -= 1;

			if(input.KeyOnce(VKey_CONTROL) || input.KeyDown(VKey_SPACE) || input.KeyOnce('J'))
			{
				if(inputFlags.jump != 2)
					inputFlags.jump++;
			}
			if(input.KeyOnce(VKey_RETURN)) inputFlags.suicide = 1;
			if(input.KeyUp(VKey_SPACE))
				inputFlags.jump = 0;
		}
		inputFlags.vMouse = input.GetMousePosition();

		if(!bRace)
		{
			entityManager.player.pos.x /= lvl.stepSizeX;
			entityManager.player.pos.y /= lvl.stepSizeY;
			entityManager.player.velocity.x /= lvl.stepSizeX;
			entityManager.player.velocity.y /= lvl.stepSizeY;

			//gGhost.CheckInput(input, entMgr.player, fTimeStamp);

			entityManager.player.pos.x *= lvl.stepSizeX;
			entityManager.player.pos.y *= lvl.stepSizeY;
			entityManager.player.velocity.x *= lvl.stepSizeX;
			entityManager.player.velocity.y *= lvl.stepSizeY;
		}
	}

	if(bRace || bPlayBack)
	{
		if(fTimeStamp < 0.0f)
		{
			if(fTimeStamp + fDelta > 0.0f)
			{
				entityManager.SpawnPlayer(entityManager.player);

				if(bRace)
				{
					for(unsigned int i = 0; i < numGhosts; ++i)
						entityManager.SpawnPlayer(gp[i]);

					entityManager.SpawnPlayer(entityManager.ghost);
				}

				fTimeStamp = 0.0f;
			}
		}


		if(!bRace)
		{
			//if(gGhost.GetInput(input, entMgr.player, fTimeStamp))
			//{
			//	entMgr.player.pos.x *= lvl.stepSizeX;
			//	entMgr.player.pos.y *= lvl.stepSizeY;
			//	entMgr.player.velocity.x *= lvl.stepSizeX;
			//	entMgr.player.velocity.y *= lvl.stepSizeY;
			//}
		}
	}

	entityManager.UpdatePlayer(fDelta, inputFlags, entityManager.player);
	entityManager.Update(fDelta, entityManager.player);

	if(bRace)
	{
		//for(unsigned int i = 0; i < numGhosts; ++i)
		//{
		//	lvl.SetPage(gp[i].pageX, gp[i].pageY, gp[i]);

		//	if(g[i].GetInput(g[i].ghostInput, gp[i], gp[i].fTimeElapsed - 0.05f * float(i)))
		//	{
		//		gp[i].pos.x *= lvl.stepSizeX;
		//		gp[i].pos.y *= lvl.stepSizeY;
		//		gp[i].velocity.x *= lvl.stepSizeX;
		//		gp[i].velocity.y *= lvl.stepSizeY;
		//	}

		//	entMgr.UpdatePlayer(fDelta, g[i].ghostInput, gp[i]);
		//	entMgr.Update(fDelta, gp[i]);
		//}

		//lvl.SetPage(entMgr.ghost.pageX, entMgr.ghost.pageY, entMgr.ghost);

		//if(gGhost.GetInput(gInput, entMgr.ghost, fTimeStamp))
		//{
		//	entMgr.ghost.pos.x *= lvl.stepSizeX;
		//	entMgr.ghost.pos.y *= lvl.stepSizeY;
		//	entMgr.ghost.velocity.x *= lvl.stepSizeX;
		//	entMgr.ghost.velocity.y *= lvl.stepSizeY;
		//}

		//entMgr.UpdatePlayer(fDelta, gInput, entMgr.ghost);
		//entMgr.Update(fDelta, entMgr.ghost);

		//lvl.SetPage(entMgr.player.pageX, entMgr.player.pageY, entMgr.player);
	}
}


void ActivitySideScroller::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	static float scale = 0.25f;
	float scalex2 = scale * 2.0f;

	float camX = floor(entityManager.player.pos.x - WindowState::Width() * scale);
	if(camX < 0.0f)
		camX += 0 - camX;

	float camW = floor(camX + WindowState::Width() * scalex2);
	if(camW > WindowState::Width())
	{
		float offset = camW - WindowState::Width();
		camW -= offset;
		camX -= offset;
	}


	float camY = floor(entityManager.player.pos.y - WindowState::Height() * scale);
	if(camY < 0.0f)
		camY += 0 - camY;

	float camH = floor(camY + WindowState::Height() * scalex2);

	if(camH > WindowState::Height())
	{
		float offset = camH - WindowState::Height();
		camH -= offset;
		camY -= offset;
	}

	//input.PushOrtho();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	glOrtho(camX, camW, camH, camY, -2, 2);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();



	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);


	glBegin(GL_QUADS);
	glColor3f(0.1f, 0.1f, 0);     glVertex3f(0, 0, -1.5f);
	glVertex3f((float)WindowState::Width(), 0, -1.5f);
	glColor3f(0.1f, 0.1f, 0.35f); glVertex3f((float)WindowState::Width(), (float)WindowState::Height(), -1.5f);
	glVertex3f(0, (float)WindowState::Height(), -1.5f);
	glEnd();

	lvl.PushRenderState();
	lvl.RenderEntities(entityManager.player.spawnerID);
	lvl.RenderLevel();
	lvl.PopRenderState();

	glLineWidth(1);

	if(bRace)
	{
		if(entityManager.player.pageX == entityManager.ghost.pageX && entityManager.player.pageY == entityManager.ghost.pageY)
		{
			glColor3f(0, 1, 0);
			entityManager.ghost.Render(lvl.halfStepX, lvl.halfStepY);
		}

		for(unsigned int i = 0; i < numGhosts; ++i)
		{
			if(entityManager.player.pageX == gp[i].pageX && entityManager.player.pageY == gp[i].pageY)
			{
				glColor3f(0, 1, 0);
				gp[i].Render(lvl.halfStepX, lvl.halfStepY);
			}
		}
	}

	glColor3f(1, 1, 1);
	if(bPlayBack)
		glColor3f(1, 0, 0);

	entityManager.player.Render(lvl.halfStepX, lvl.halfStepY);

	if(bShowJumps)
	{
		glm::vec2 verts[4] = {glm::vec2(-0.5f, -0.5f), glm::vec2(0.5f, -0.5f), glm::vec2(0.5f, 0.5f), glm::vec2(-0.5f, 0.5f)};
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(2, GL_FLOAT, 0, &verts->x);

		float scaleX = entityManager.player.size.x * lvl.stepSizeX;
		float scaleY = entityManager.player.size.y * lvl.stepSizeY;
		for(unsigned int i = 0; i < entityManager.failVec.size(); ++i)
		{
			glPushMatrix();
			glColor3f(0.21f, 0.21f, 0);
			glTranslatef(entityManager.failVec[i].x, entityManager.failVec[i].y, 0);
			glScalef(scaleX, scaleY, 1);
			glDrawArrays(GL_LINE_LOOP, 0, 4);
			glPopMatrix();
		}

		for(unsigned int i = 0; i < entityManager.spaceVec.size(); ++i)
		{
			glPushMatrix();
			glColor3f(0.51f, 0.51f, 0);
			glTranslatef(entityManager.spaceVec[i].x, entityManager.spaceVec[i].y, 0);
			glScalef(scaleX, scaleY, 1);
			glDrawArrays(GL_LINE_LOOP, 0, 4);
			glPopMatrix();
		}
	}


	glPointSize(2.0f);

	glColor3f(1, 1, 1);

	auto& text = Services().Text();

	if(entityManager.bInfo)
	{
		glEnable(GL_DEPTH_TEST);

		if(entityManager.InfoID <= 10)
		{
			text.Print(entityManager.player.pos.x, entityManager.player.pos.y - entityManager.player.size.y * lvl.stepSizeY, sInfo[entityManager.InfoID], 22, glm::vec3(0.3, 1, 0.3), true);
		}

		glDisable(GL_DEPTH_TEST);
	}

	glColor3f(1, 1, 1);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();


	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	float minutesElapsed = floor(entityManager.player.fTimeElapsed / 60.0f);
	float secondsElapsed = entityManager.player.fTimeElapsed - (minutesElapsed * 60.0f);
	text.Print(0, 0, fmt::format("FPS: %i      Sound: %i     Deaths:%i       Coins %i/%i      Time: %.0f:%.1f    Spawn %i (%i)", 0, 0, entityManager.killCounter, entityManager.coinCounter, lvl.coinsTotal, minutesElapsed, secondsElapsed, entityManager.player.spawnerID, entityManager.spawnerCounter), 16, Colors::White);


	//if(bRace)
	//{
	//	float gminutesElapsed = floor(entMgr.ghost.fTimeElapsed / 60.0f);
	//	float gsecondsElapsed = entMgr.ghost.fTimeElapsed - (gminutesElapsed * 60.0f);
	//	glColor3f(0, 1, 0);
	//	dbgText.PrintNow(0, 40, "Ghost Time: %.0f:%.1f", gminutesElapsed, gsecondsElapsed);

	//	if(fTimeStamp < 0.0f)
	//	{
	//		dbgText.PrintNow(input.Width / 2.0f, input.Height / 2.0f, "%.0f", fTimeStamp);
	//	}
	//}


	//if(showDEBUG)
	//{
	//	dbgText.PrintNow(0, 20, "Ghost: e(%i) mem(%i B)  Playback(%i)  Race(%i)", gGhost.inputVec.size(), gGhost.inputVec.size() * sizeof(sInputChange), bPlayBack, bRace);
	//	dbgText.PrintNow(0, 130, "Ghost(%.2f, %.2f)(%i, %i), Velocity(%f, %f) Page(%i,%i)", entMgr.ghost.pos.x, entMgr.ghost.pos.y, entMgr.level->toGridX(entMgr.ghost.pos), entMgr.level->toGridY(entMgr.ghost.pos), entMgr.ghost.velocity.x, entMgr.ghost.velocity.y, entMgr.ghost.pageX, entMgr.ghost.pageY);
	//	dbgText.PrintNow(0, 150, "Player(%.2f, %.2f)(%i, %i), Velocity(%f, %f) Page(%i,%i)", entMgr.player.pos.x, entMgr.player.pos.y, entMgr.level->toGridX(entMgr.player.pos), entMgr.level->toGridY(entMgr.player.pos), entMgr.player.velocity.x, entMgr.player.velocity.y, entMgr.player.pageX, entMgr.player.pageY);
	//	dbgText.PrintNow(0, 170, "gridSize(%f, %f)  memUsage(%i)", lvl.stepSizeX, lvl.stepSizeY, lvl.CalculateMemoryUsed());
	//	dbgText.PrintNow(0, 190, "%i %i", lvl.HorizontalCheck(entMgr.player.pos, entMgr.player.size), lvl.WallCheck(entMgr.player.pos, entMgr.player.size));
	//	dbgText.PrintNow(0, 210, "Flags: grnd(%i) lad(%i) sl(%i) db(%i) dju(%i) wju(%i) grav(%i)", entMgr.player.checkFlag(ENT_GROUNDED), entMgr.player.checkFlag(ENT_LADDERING), entMgr.player.checkFlag(ENT_SLIDING), entMgr.player.checkFlag(ENT_DBLJUMP), entMgr.player.checkFlag(ENT_DBLJUMPUPGRADE), entMgr.player.checkFlag(ENT_WALLJUMPUPGRADE), entMgr.player.checkFlag(ENT_GRAVITY));
	//	PROFILERENDER
	//}


	if(entityManager.ghost.pageX == 0 && entityManager.ghost.pageY == 1) 
		entityManager.ghost.bFinished = true;

	//static float fTimer = 0.0f; //lazy hack
	//if(!entMgr.bGameEnded)
	//	fTimer = 0.0f;

	//if(lvl.page_x == 0 && lvl.page_y == 1)
	//{
	//	unsigned int statusbckp = p.Achievements; //to avoid ghosts getting achievements

	//	fTimer += 1 / 60.0f;

	//	entMgr.bGameEnded = true;
	//	entMgr.player.bFinished = true;

	//	unsigned int timeUsed = (unsigned int)entMgr.player.fTimeElapsed;

	//	dbgText.bCenter = true;

	//	float centerX = input.Width / 2.0f;
	//	float centerY = input.Height / 2.0f - 200.0f;

	//	glColor3f(0.5f, 1, 0.5f);
	//	if(fTimer > 1.0f) dbgText.PrintNow(input.Width / 2.0f, centerY, "Congratulations!");
	//	if(fTimer > 1.5f) dbgText.PrintNow(input.Width / 2.0f, centerY + 20, "You beat the game");

	//	if(fTimer > 2.0f) dbgText.PrintNow(input.Width / 2.0f, centerY + 60, "Rewards:");

	//	glColor3f(1, 0.5f, 0.5f);
	//	float offset = 80.0f;
	//	int rewardValue = 0;

	//	if(fTimer > 2.3f)
	//	{
	//		dbgText.PrintNow(centerX, centerY + offset, "The End?");
	//		offset += 20.0f;

	//		p.Achievements |= 1;
	//	}

	//	if(fTimer > 2.5f && entMgr.player.spawnerID == -1)
	//	{
	//		p.Achievements |= 256;
	//		dbgText.PrintNow(centerX, centerY + offset, "Insanity!!!!");
	//		offset += 20.0f;

	//		rewardValue++;
	//	}

	//	if(fTimer > 3.0f)
	//	{
	//		if(entMgr.killCounter < 10)
	//		{
	//			p.Achievements |= 2;

	//			dbgText.PrintNow(centerX, centerY + offset, "Challenge Accepted. (<10Deaths)");
	//			offset += 20.0f;
	//			rewardValue += 1;
	//		}


	//		if(fTimer > 3.2f && entMgr.killCounter < 5)
	//		{
	//			p.Achievements |= 4;
	//			dbgText.PrintNow(centerX, centerY + offset, "Nearly there!  (<5Deaths)");
	//			offset += 20.0f;
	//			rewardValue += 2;
	//		}

	//		if(fTimer > 3.4f && entMgr.killCounter == 0)
	//		{
	//			p.Achievements |= 8;
	//			dbgText.PrintNow(centerX, centerY + offset, "Spikephobia!! (0Deaths)");
	//			offset += 20.0f;
	//			rewardValue += 3;
	//		}
	//	}
	//	if(fTimer > 3.6f)
	//	{
	//		if(entMgr.coinCounter == lvl.coinsTotal)
	//		{
	//			p.Achievements |= 16;
	//			dbgText.PrintNow(centerX, centerY + offset, "Master of Coins");
	//			offset += 20.0f;
	//			rewardValue += 2;
	//		}

	//		if(entMgr.coinCounter > lvl.coinsTotal - 14)
	//		{
	//			p.Achievements |= 32;
	//			dbgText.PrintNow(centerX, centerY + offset, "Lord of Coins");
	//			offset += 20.0f;
	//			rewardValue += 1;
	//		}
	//	}
	//	if(fTimer > 4.0f)
	//	{
	//		if(timeUsed < 300 && entMgr.coinCounter == lvl.coinsTotal)
	//		{
	//			p.Achievements |= 64;
	//			dbgText.PrintNow(centerX, centerY + offset, "Speed King! (%i seconds)", timeUsed);
	//			offset += 20.0f;

	//			rewardValue += 1;
	//		}

	//		if(timeUsed < 170)
	//		{
	//			p.Achievements |= 128;
	//			dbgText.PrintNow(centerX, centerY + offset, "What's the rush?", timeUsed);
	//			offset += 20.0f;

	//			rewardValue += 1;
	//		}
	//	}

	//	if(fTimer > 5.0f)
	//	{
	//		if(rewardValue >= 10)
	//		{
	//			p.Achievements |= 512;
	//			offset += 40.0f;
	//			glColor3f(1, 0, 0);
	//			dbgText.PrintNow(centerX, centerY + offset, "GODLIKE!");
	//		}
	//	}

	//	if(bPlayBack)
	//		p.Achievements = statusbckp;

	//	dbgText.bCenter = false;
	//}

	glColor3f(1, 1, 1);
}
