#include"EntityManager.h"

EntityManager::EntityManager()
{
	spaceVec.reserve(1000);
	failVec.reserve(1000);

	ghost.bGhost = true;
}

EntityManager::~EntityManager()
{
}


void EntityManager::SpawnPlayer(Entity& plr, int spawnID)
{
	plr.maxvelocity = glm::vec2(18.5714, 999);
	plr.acceleration = glm::vec2(15.4761, 9.8754);
	plr.walljump = glm::vec2(100.0f / 16.153847f, plr.acceleration.y);
	plr.friction = glm::vec2(30.9523, 0);
	plr.size = glm::vec2(0.9f, 2 * 0.9f);
	plr.tryMove = glm::vec2(0, 0);
	plr.fTimeElapsed = 0.0f;
	plr.bFinished = false;
	plr.spawnerID = spawnID;

	if(spawnID != -1)
	{
		//etc
		Spawner* s = level->getSpawner(spawnID);
		level->SpawnEntity(s->x, s->y, plr);
		level->SetPage(s->pageX, s->pageY, plr);

		plr.pageX = s->pageX;
		plr.pageY = s->pageY;
	}
	else
	{
		plr.pageX = (level->startX & 0xFFFF0000) >> 16;
		plr.pageY = (level->startY & 0xFFFF0000) >> 16;

		level->SetPage(plr.pageX, plr.pageY, plr);
		level->SpawnEntity(level->startX & 0x0000FFFF, level->startY & 0x0000FFFF, plr);
	}


	plr.Status = 0;
	plr.flag(ENT_GRAVITY);

	plr.spawnFlags = plr.Status;

	plr.bLeft = false;
}


void EntityManager::Update(float fDelta, Entity& plr)
{
	//MOVEMENT
	plr.tryMove = glm::vec2(plr.velocity * fDelta);

	if(fabs(plr.tryMove.x) < 0.05f) plr.tryMove.x = 0;
	if(fabs(plr.tryMove.y) < 0.05f) plr.tryMove.y = 0;


	if(plr.checkFlag(ENT_GROUNDED) && plr.tryMove.y > 0.0f) //if onGround && velocity is pointing towards the ground
		plr.tryMove.y = 0.0f;

	if(fabs(plr.tryMove.x) > 0.0f)
	{
		int wallCheck = level->WallCheck(plr.pos, plr.size);

		if(wallCheck == 1 && plr.velocity.x > 0.0f)
			plr.tryMove.x = 0;
		if(wallCheck == -1 && plr.velocity.x < 0.0f)
			plr.tryMove.x = 0;
	}


	glm::vec2 end = plr.pos + plr.tryMove;

	float fraction = 1.0f;
	fraction = level->BoxGridCheck(plr.pos, end, plr.size);

	glm::vec2 isect(0, 0);
	glm::vec2 nm = normalize(plr.tryMove);
	isect = plr.pos + plr.tryMove * fraction;


	if(fraction < 1.0f)
		isect -= nm * 0.01f;

	plr.pos = isect;

	glm::vec2 vc;
	if(plr.velocity.x > 0.0f)
		vc = glm::vec2(0.1f, 0);//Right Wall
	else
		vc = glm::vec2(-0.1f, 0);//Left Wall

	if(level->BoxGridCheck(plr.pos, plr.pos + vc, plr.size) < 1.0f)
		plr.velocity.x = 0;


	if(plr.velocity.y > 0)//GroundCheck
		vc = glm::vec2(0, 0.1f);
	else
		vc = glm::vec2(0, -0.1f);//CeilCheck

	plr.unflag(ENT_GROUNDED);
	if(level->BoxGridCheck(plr.pos, plr.pos + vc, plr.size) < 1.0f)
	{
		if(plr.velocity.y > 0)
			plr.flag(ENT_GROUNDED);
		else if(plr.velocity.y < 0)
			plr.unflag(ENT_GROUNDED);

		plr.velocity.y = 0;
	}

	//PAGECHECK
	bool bPageFlip = true;
	if((int)(plr.pos.x / level->stepSizeX) >= 64 && plr.velocity.x > 0.0f)
	{
		if(level->SetPage(level->page_x + 1, level->page_y, plr))
			plr.pos.x = 1.0f * level->stepSizeX;
	}
	else if((int)(plr.pos.x / level->stepSizeX) <= 1 && plr.velocity.x < 0.0f)
	{
		if(level->SetPage(level->page_x - 1, level->page_y, plr))
			plr.pos.x = 64.0f * level->stepSizeX;

	}
	else if((int)(plr.pos.y / level->stepSizeY) >= 36 && plr.velocity.y > 0.0f)
	{
		if(level->SetPage(level->page_x, level->page_y + 1, plr))
			plr.pos.y = 1 * level->stepSizeY;
	}
	else if((int)(plr.pos.y / level->stepSizeY) <= 1 && plr.velocity.y < 0.0f)
	{
		if(level->SetPage(level->page_x, level->page_y - 1, plr))
			plr.pos.y = 36.0f * level->stepSizeY;
	}
	else
		bPageFlip = false;

	if(bPageFlip)
	{
		plr.pageX = level->page_x;
		plr.pageY = level->page_y;
		spaceVec.clear();
		failVec.clear();
	}
}


float fGravity = (500.0f / 16.153847f);



//maybe move this one to class Player : public Entity (?)
void EntityManager::UpdatePlayer(float fDelta, sInputFlags& input, Entity& player)
{
	if(!player.bFinished)
		player.fTimeElapsed += fDelta;

	bGodMode = false;

	if(input.click[0] && bGodMode)
	{
		player.pos = glm::vec2(input.vMouse);

		player.velocity = glm::vec2(0, 0);
		player.tryMove = glm::vec2(0, 0);
		player.flag(ENT_GROUNDED);
	}

	if(player.checkFlag(ENT_GROUNDED))
		player.unflag(ENT_DBLJUMP);

	//Jump.
	static float jumpTimer = 0.0f;
	if(input.jump)
	{
		//spaceVec.push_back(plr.pos);

		//if on ground, jump.
		if(player.checkFlag(ENT_GROUNDED))
		{
			player.velocity.y = -player.acceleration.y * level->stepSizeY;

			spaceVec.push_back(player.pos);
			player.flag(ENT_INJUMP);

			jumpTimer = 0.0f;
		}
		else if(player.checkFlag(ENT_INJUMP))
		{
			jumpTimer += fDelta;

			if(jumpTimer < 0.2f)
			{
				player.velocity.y = -player.acceleration.y * level->stepSizeY;
			}
			else
				player.unflag(ENT_INJUMP);
		}
		else if(input.jump == 1)
		{
			bool bJumped = false;
			bool canWallJump = player.checkFlag(ENT_WALLJUMPUPGRADE);
			bool canDoubleJump = true;


			if(canWallJump)
			{
				int wallCheck = level->WallCheck(player.pos, player.size);
				canDoubleJump = true;

				if(wallCheck != 0)
				{
					if(input.moveVertical != 0 && input.moveHorizontal == 0)
					{
						player.velocity.x = -player.walljump.x * level->stepSizeX * 2.0f * float(wallCheck);
						player.velocity.y = -player.walljump.y * level->stepSizeY * 0.0f;
					}
					else if(input.moveHorizontal == 1 && input.moveVertical == 0)
					{
						player.velocity.x = -player.walljump.x * level->stepSizeX * 1.5f * float(wallCheck);
						player.velocity.y = -player.walljump.y * level->stepSizeY * 0.5f;
					}
					else
					{
						player.velocity.y = -player.walljump.y * level->stepSizeY;
						player.velocity.x = -player.walljump.x * level->stepSizeX * float(wallCheck);
					}

					player.unflag(ENT_DBLJUMP);

					//if(!plr.bGhost)
					//	sndMgr->playSound(4);

					canDoubleJump = false;
					bJumped = true;
				}

				if(wallCheck != 0 && !player.bGhost)
					spaceVec.push_back(player.pos);
			}

			if(canDoubleJump && !player.checkFlag(ENT_DBLJUMP) && player.checkFlag(ENT_DBLJUMPUPGRADE))
			{
				player.velocity.y = -player.acceleration.y * level->stepSizeY;
				player.flag(ENT_DBLJUMP);

				if(!player.bGhost)
				{
					//sndMgr->playSound(5);
					spaceVec.push_back(player.pos);
				}
				bJumped = true;
			}

			if(!bJumped && !player.bGhost)
				failVec.push_back(player.pos);
		}
	}
	else
	{
		player.unflag(ENT_INJUMP);
	}

	//Calculates current position in the grid and checks for stuff that can be interacted with (ie pickups or checkpoints)
	glm::ivec2 playerPos{level->toGridX(player.pos), level->toGridY(player.pos)};

	int colMapIndex = playerPos.x + playerPos.y * level->curPage->width;
	int colMapValue = level->curPage->colMap[colMapIndex];
	bInfo = false;

	//Check to see if we're still touching a ladder
	if(player.checkFlag(ENT_LADDERING))
	{
		if(colMapValue != 2)
			player.unflag(ENT_LADDERING);
	}


	if(colMapValue == 101 || input.suicide) //spike
	{
		Item* i = level->getItem(playerPos.x, playerPos.y);

		if(i)
			i->sprite = level->getSprite(i->sprite->x, 3);

		Spawner* s = level->getSpawner(player.spawnerID);

		//change to startspawn
		int sX = 4;
		int sY = 31;

		if(s)
		{
			sX = s->x;
			sY = s->y;

			level->SetPage(s->pageX, s->pageY, player);

			player.pageX = s->pageX;
			player.pageY = s->pageY;

		}
		else
		{
			sX = level->startX & 0x0000FFFF;
			sY = level->startY & 0x0000FFFF;
			player.pageX = (level->startX & 0xFFFF0000) >> 16;
			player.pageY = (level->startY & 0xFFFF0000) >> 16;

			level->SetPage(player.pageX, player.pageY, player);
		}

		level->SpawnEntity(sX, sY, player);

		if(!player.bGhost)
		{
			player.Status = player.spawnFlags;
			coinCounter = prevCoins;

			for(unsigned int i = 0; i < pickups.size(); ++i)
				pickups[i]->bActive = true;

			pickups.clear();
		}

		if(!player.bGhost)
		{
			killCounter++;
			//sndMgr->playSound(2);
		}
	}

	player.friction.x = 30.9523f;
	player.acceleration.x = 15.4761;
	player.flag(ENT_GRAVITY);

	if(colMapValue != 0)
	{
		if(colMapValue == 55)
		{
			Item* i = level->getItem(playerPos.x, playerPos.y);
			if(i->bActive)
			{
				if(!player.bGhost)
				{
					//sndMgr->playSound(1);
					coinCounter++;
					pickups.push_back(i);
					level->remItem(playerPos.x, playerPos.y);//THIS WILL ONLY SET THE ITEM TO INACTIVE
				}
			}
		}
		else if(colMapValue == 1300 && !player.checkFlag(ENT_WALLJUMPUPGRADE))
		{
			Item* i = level->getItem(playerPos.x, playerPos.y);
			if(i->bActive)
			{
				player.flag(ENT_WALLJUMPUPGRADE);

				if(!player.bGhost)
				{
					level->remItem(playerPos.x, playerPos.y); //THIS WILL ONLY SET THE ITEM TO INACTIVE
					pickups.push_back(i);
					//sndMgr->playSound(3);
				}
			}
		}
		else if(colMapValue == 1301 && !player.checkFlag(ENT_DBLJUMPUPGRADE))
		{
			Item* i = level->getItem(playerPos.x, playerPos.y);
			if(i->bActive)
			{
				player.flag(ENT_DBLJUMPUPGRADE);

				if(!player.bGhost)
				{
					//sndMgr->playSound(3);
					pickups.push_back(i);
					level->remItem(playerPos.x, playerPos.y);//THIS WILL ONLY SET THE ITEM TO INACTIVE
				}
			}
		}
		else if(colMapValue == 1299)
		{
			Spawner* s = level->getSpawner(playerPos.x, playerPos.y);

			if(s)
			{
				if(player.spawnerID != s->ID)
				{
					if(!player.bGhost)
					{
						//sndMgr->playSound(6);
						spawnerCounter++;
						prevCoins = coinCounter;
						for(unsigned int i = 0; i < pickups.size(); ++i)
							prevPickups.push_back(pickups[i]);

						pickups.clear();
					}

					player.spawnFlags = player.Status;
				}

				player.spawnerID = s->ID;
			}
		}
		else if(((colMapValue >= 1500 && colMapValue <= (1500 + 254)) || colMapValue == 1701) && !player.bGhost)
		{
			InfoID = colMapValue - 1500;
			bInfo = true;
		}
		else if(colMapValue == 136)
		{
			player.friction.x = 3;
			player.acceleration.x = 2;
		}
		else if(colMapValue == 137)
		{
			player.acceleration.x = 50;
		}
		else if(colMapValue == 301)
		{
			player.unflag(ENT_GRAVITY);
		}
	}

	int plrPosXE0 = level->toGridX(player.pos.x + level->halfStepX * player.size.x);
	int plrPosYE0 = level->toGridY(player.pos.y + level->halfStepY * player.size.y);
	int colMapIndexE0 = plrPosXE0 + plrPosYE0 * level->curPage->width;
	int colMapValueE0 = level->curPage->colMap[colMapIndexE0];

	int plrPosXE1 = level->toGridX(player.pos.x - level->halfStepX * player.size.x);
	int plrPosYE1 = level->toGridY(player.pos.y + level->halfStepY * player.size.y);
	int colMapIndexE1 = plrPosXE1 + plrPosYE1 * level->curPage->width;
	int colMapValueE1 = level->curPage->colMap[colMapIndexE1];

	if(colMapValueE0 == 3 || colMapValueE1 == 3)
	{
		if(colMapValueE0 == 3)
		{
			Item* i = level->getItem(plrPosXE0, plrPosYE0);
			if(i && i->bActive)
			{
				if(!player.bGhost)
				{
					pickups.push_back(i);
					i->bActive = false;
					//level->remItem(plrPosXE0, plrPosYE0);//THIS WILL ONLY SET THE ITEM TO INACTIVE
				}
			}
		}

		if(colMapValueE1 == 3)
		{
			Item* i = level->getItem(plrPosXE1, plrPosYE1);
			if(i && i->bActive)
			{
				if(!player.bGhost)
				{
					pickups.push_back(i);
					level->remItem(plrPosXE1, plrPosYE1);//THIS WILL ONLY SET THE ITEM TO INACTIVE
				}
			}
		}
	}


	//TODO: cancel movement when hitting wall\ceil when laddering
	int horiCheck = level->HorizontalCheck(player.pos, player.size);

	if(input.moveVertical == 1 && horiCheck != -1)
	{
		if(colMapValue == 2)
		{
			player.flag(ENT_LADDERING);
			player.velocity.y = -player.acceleration.y * level->stepSizeY;
		}
	}
	else if(input.moveVertical == -1 && horiCheck != 1)
	{
		if(colMapValue == 2)
		{
			player.flag(ENT_LADDERING);
			player.velocity.y = player.acceleration.y * level->stepSizeY;
		}
	}
	else if(player.checkFlag(ENT_LADDERING))
		player.velocity.y = 0;


	//Horizontal Movement on ground or ladders
	//movement can be cancelled below
	player.unflag(ENT_SLIDING);
	bool bMovement = false;
	//if( input.moveHorizontal == 1 && (plr.checkFlag(ENT_GROUNDED) || plr.checkFlag(ENT_LADDERING)) )
	if(input.moveHorizontal == 1)
	{
		if(player.velocity.x < 0.0f)
		{
			player.flag(ENT_SLIDING);
			player.velocity.x += player.friction.x * level->stepSizeX * fDelta;

			if(player.velocity.x > 0.0f)
				player.velocity.x = 0.0f;
		}

		bMovement = true;

		if(player.checkFlag(ENT_GROUNDED) || player.checkFlag(ENT_LADDERING))
			player.velocity.x += player.acceleration.x * level->stepSizeX * fDelta;

		if(player.velocity.x > player.maxvelocity.x * level->stepSizeX)
			player.velocity.x = player.maxvelocity.x * level->stepSizeX;
	}

	//if( input.moveHorizontal == -1 && (plr.checkFlag(ENT_GROUNDED) || plr.checkFlag(ENT_LADDERING)) )
	if(input.moveHorizontal == -1)
	{
		if(player.velocity.x > 0.0f)
		{
			player.flag(ENT_SLIDING);
			player.velocity.x -= player.friction.x * level->stepSizeX * fDelta;

			if(player.velocity.x < 0.0f)
				player.velocity.x = 0.0f;
		}

		bMovement = true;

		if(player.checkFlag(ENT_GROUNDED) || player.checkFlag(ENT_LADDERING))
			player.velocity.x -= player.acceleration.x * level->stepSizeX * fDelta;

		if(player.velocity.x < -player.maxvelocity.x * level->stepSizeX)
			player.velocity.x = -player.maxvelocity.x * level->stepSizeX;
	}

	if(!bMovement && player.checkFlag(ENT_LADDERING))
		player.velocity.x = 0;

	if(player.checkFlag(ENT_LADDERING))
	{
		//horizontal check for ladders
		int wc = level->WallCheck(player.pos, player.size);
		if(input.moveHorizontal == 1 && wc == 1)
			player.velocity.x = 0;
		if(input.moveHorizontal == -1 && wc == -1)
			player.velocity.x = 0;
	}


	//Friction
	if(!bMovement && player.checkFlag(ENT_GROUNDED))
	{
		if(player.velocity.x > 0.0f)
		{
			player.velocity.x -= player.friction.x * level->stepSizeX * fDelta;
			if(player.velocity.x <= 0)
				player.velocity.x = 0;

			player.flag(ENT_SLIDING);
		}
		else if(player.velocity.x < 0.0f)
		{
			player.velocity.x += player.friction.x * level->stepSizeX * fDelta;
			if(player.velocity.x >= 0.0f)
				player.velocity.x = 0.0f;

			player.flag(ENT_SLIDING);
		}
	}

	//Gravity
	if(player.checkFlag(ENT_GRAVITY) && !player.checkFlag(ENT_LADDERING))
		player.velocity.y += fGravity * level->stepSizeY * fDelta;
}


void EntityManager::Render()
{
}
