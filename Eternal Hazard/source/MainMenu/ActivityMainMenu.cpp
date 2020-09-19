#include"pch/pch.h"
#include"ActivityMainMenu.h"

//#include"Animator.h"
//#include"SS.h"
//#include"ClientMgr.h"

#include"Menu.h"

#include<vector>
#include<string>

using std::string;
using std::vector;

//SS* ss = 0;


//Profile p;


struct MenuObject
{
	string title;
	vector<string> strings;
};

MenuObject objects[2];
int menuID = 0;
int pointer = 1;
bool bLoad = false;

Menu menu;

void ActivityMainMenu::Initialize()
{
	//memset(&p, 0, sizeof(Profile));

	MenuPage* mp = vnew MenuPage;
	mp->addElement("Continue", "back to the current game", 0);
	mp->addElement("New Game", "start a new game", 1);
	mp->addElement("Load Game", "continue your previous game", 2);
	mp->addElement("Achievements", "view your achievements", 3);
	mp->addElement("Options", "tinker with them options", 4);
	mp->addElement("DevTools", "Animator, no touchietouchie, very unstable", 5);
	mp->addElement("About", "aboot", 6);
	mp->addElement("Exit", "whyyyy? :(", 7);
	mp->title = "rawr";
	mp->parentMenuID = -1;
	mp->ID = 0;
	mp->selection = 1;
	menu.addMenuPage(mp);

	mp = vnew MenuPage;
	mp->addElement("End.", "Completed the game", 0);
	mp->addElement("Challenge Accepted", "Complete the level by dying less than 10 times", 1);
	mp->addElement("Nearly there!", "Complete the level by dying less than 5 times", 2);
	mp->addElement("Spikephobia", "Complete the level with no deaths", 3);
	mp->addElement("Master of Coins", "Possess all coins by the end of the level", 4);
	mp->addElement("Lord of Coins", "Have half of all coins at the end of the level", 5);
	mp->addElement("Speed King!", "Complete the level faster than 400 seconds and have all coins", 6);
	mp->addElement("What's the rush?", "Complete the level faster than 3 minutes", 7);
	mp->addElement("Insanity!", "Do not activate any checkpoints before the end", 8);
	mp->addElement("GODLIKE", "End+Spikephobia+Speed King!+Insanity!", 9);
	mp->title = "Achievements";
	mp->parentMenuID = 0;
	mp->ID = 1;
	mp->selection = 0;
	menu.addMenuPage(mp);

	mp = vnew MenuPage;
	mp->addElement("Nothing", "why not zoidberg? (V)(o,,,o)(V)", 0);
	mp->addElement("To do", "woopwoop", 1);
	mp->addElement("Here", "woop", 2);
	mp->addElement("Yet", "woops?", 2);
	mp->title = "Options";
	mp->parentMenuID = 0;
	mp->ID = 2;
	mp->selection = 0;
	menu.addMenuPage(mp);

	mp = vnew MenuPage;
	mp->addElement("Play", "start a new solo game", 0);
	mp->addElement("Race #1", "race against demo #1", 1);
	mp->addElement("Race #2", "race against demo #2", 2);
	mp->addElement("Watch Demo #1", "speed run", 3);
	mp->addElement("Watch Demo #2", "godlike run", 4);
	mp->title = "(*,,,*)";
	mp->parentMenuID = 0;
	mp->ID = 3;
	mp->selection = 0;
	menu.addMenuPage(mp);


	mp = vnew MenuPage;
	mp->addElement("VypR", "gameprogrammer", 0);
	mp->addElement("unigee", "leveleditor", 2);
	mp->addElement("CyberKenny", "music", 1);
	mp->title = "Credits";
	mp->parentMenuID = 0;
	mp->ID = 4;
	mp->selection = 0;
	menu.addMenuPage(mp);

	FILE* pFile = 0;
	fopen_s(&pFile, "data/pro.file", "rb");
}



void ActivityMainMenu::Deinitialize()
{
	menu.Destroy();
}

void ActivityMainMenu::StartGame(int type)
{
	//if(type == 0 && !ss->entMgr.bGameStarted) return;

	//bLoad = false;

	//for(int i = 0; i < 5; ++i)
	//	ss->netPlayers[i].isActive = false;

	//ss->sndMgr.playMusic(1);
	//ss->bDestroy = false;
	//ClientManager::getInstance()->PushClient(ss);
	StartActivity("ActivitySideScroller");
}

bool bExit = false;
void ActivityMainMenu::Update(float fDelta)
{
	const auto& input = Services().Input();

	if(input.KeyOnce(VKey_ESCAPE) || bExit)
	{
		if(menu.pages[menu.pageID]->parentMenuID == -1)
		{
			this->Finish();
			return;
		}

		menu.pageID = menu.pages[menu.pageID]->parentMenuID;
	}

	if(input.KeyOnce(VKey_UP))
		menu.moveSelection(-1);

	if(input.KeyOnce(VKey_DOWN))
		menu.moveSelection(1);

	if(input.KeyOnce(VKey_RETURN))
	{
		int actionID = menu.getAction();
		if(menu.pageID == 0) //MAIN MENU
		{
			if(actionID == 0) //Continue
				StartGame(0);

			if(actionID == 1) //New Game
				menu.pageID = 3;

			if(actionID == 2) //Load
			{
				//if(ss->LoadGame())
				//	StartGame(1);
			}

			if(actionID == 3) //Achievements
			{
				for(int i = 0; i < 10; ++i)
				{
					//unsigned int test = 1 << i;
					//if(p.Achievements & test)
					//	menu.pages[1]->setActive(i, true);
					//else
					//	menu.pages[1]->setActive(i, false);
				}
				menu.pageID = 1;
			}

			if(actionID == 4) //Options
				menu.pageID = 2;

			//if(actionID == 5) //devtools
			//	ClientManager::getInstance()->PushClient(new Animator(win));

			if(actionID == 6)
				menu.pageID = 4;

			if(actionID == 7) //exit
				bExit = true;
		}
		else if(menu.pageID == 1) //Achievements
		{
		}
		else if(menu.pageID == 2) //Options
		{
		}
		else if(menu.pageID == 3)
		{
			//if(actionID == 0) //NEW
			//{
			//	ss->NewGame();
			//	StartGame(1);
			//}
			//else if(actionID == 1) //RACE
			//{
			//	ss->StartRace();
			//	StartGame(1);
			//}
			//else if(actionID == 3) //REPLAY #1
			//{
			//	ss->StartReplay("Data/dev1.ghost");
			//	StartGame(1);
			//}
			//else if(actionID == 4) //REPLAY #2
			//{
			//	ss->StartReplay("Data/dev2.ghost");
			//	StartGame(1);
			//}
		}

		//ss->sndMgr.playSound(7);
	}


	//if(input.KeyOnce('C'))
	//{
	//	FILE* pFile = 0;
	//	fopen_s(&pFile, "Data/ip.txt", "rb");

	//	char ip[32] = {0};
	//	int index = 0;
	//	while(!feof(pFile))
	//	{
	//		char c = 0;
	//		fread(&c, 1, 1, pFile);

	//		if(c == ':')
	//			break;

	//		ip[index] = c;
	//		index++;
	//	}

	//	fclose(pFile);

	//	ss->net.Initialize(false);
	//	ss->net.SetHook(ss);
	//	RakNet::ConnectionAttemptResult car = ss->net.peer->Connect(ip, 10101, 0, 0);
	//	ss->NewGame();
	//	StartGame(1);
	//}

	//if(win.KeyOnce('S'))
	//{
	//	ss->net.Initialize(true);
	//	ss->net.SetHook(ss);
	//	ss->NewGame();
	//	StartGame(1);
	//}
}


#include"GL/glew.h"
#include"glm/glm.hpp"
void ActivityMainMenu::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	//win.PushOrtho();
	glBegin(GL_QUADS);
	glColor3f(0.2f, 0, 0); 
	glVertex3f(0, 0, -1.5); 
	glVertex3f(float(WindowState::Width()), 0, -1.5);
	glColor3f(0, 0, 0.1f); 
	glVertex3f((float)WindowState::Width(), (float)WindowState::Height(), -1.5); 
	glVertex3f(0, (float)WindowState::Height(), -1.5);
	glEnd();

	glEnable(GL_DEPTH_TEST);
	glColor3f(1, 1, 1);
	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glVertex2f(50, 100);
	glVertex2f(50, 200);
	glEnd();

	static float r = 0;
	r += 3 * 1 / 60.0f;


	glColor3f(1, 0, 0);
	glPointSize(5.0f);
	glBegin(GL_POINTS);
		glVertex3f(50 + cos(r) * 10.0f, 150, sin(r));
	glEnd();

	glDisable(GL_DEPTH_TEST);

	float halfWidth = WindowState::Width() / 2.0f;
	glColor3f(0, 1, 0);

	auto point = glm::vec2((double)halfWidth, WindowState::Height() / 2.0);

	auto& text = Services().Text();
	menu.Render(point, text, text, text);

	//if(menu.pageID == 0)
	//{
	//	float xValue = win.Width - 200.0f;

	//	txtOption.bCenter = true;

	//	glColor3f(0.5f, 0.5f, 0.95f);
	//	txtOption.PrintNow(xValue, 200.0f, "Stats");

	//	glColor3f(0.4f, 0.4f, 0.95f);
	//	txtInfo.bCenter = true;
	//	txtInfo.PrintNow(xValue, 230.0f, "Level Completions: %i", p.LevelCompletions);
	//	txtInfo.PrintNow(xValue, 250.0f, "Total Deaths: %i", p.TotalDeaths);
	//	txtInfo.PrintNow(xValue, 270.0f, "Coins Collected: %i", p.NumCoins);
	//	txtInfo.PrintNow(xValue, 290.0f, "Highest Rank Achieved(NYI): %i", p.HighestRank);
	//	txtInfo.PrintNow(xValue, 310.0f, "Total Time: %i seconds", p.TotalTimeElapsed);

	//	txtInfo.bCenter = false;
	//}

	text.Print(0, double(WindowState::Height()), "Version 5.2.1 alpha", 18, glm::vec3(0.3f, 0.4f, 1), false);
}

