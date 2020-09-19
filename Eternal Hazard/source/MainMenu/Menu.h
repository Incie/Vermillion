#pragma once


#include"glm/glm.hpp"
#include<string>
#include<vector>

class TextService;

struct MenuAction
{
	std::string title;
	std::string info;
	unsigned int ID;
	bool active;
};

class MenuPage
{
public:
	MenuPage();
	~MenuPage();

	void movePointer(int dir);
	void mouseCheck(unsigned int mouseX, unsigned int mouseY);
	int getAction();
	//void purge
	void addElement(std::string title, std::string info, unsigned int ID, bool bActive = true);
	void setActive(unsigned int actionID, bool bActive);

	void Render(glm::vec2& center, const TextService& big, const TextService& action, const TextService& info);
	void Destroy();


	std::string title;
	std::vector< MenuAction > actions;
	unsigned int ID;
	unsigned int parentMenuID;
	int selection;
};


class Menu
{
public:
	Menu();
	~Menu();

	void addMenuPage(MenuPage* p);
	void moveSelection(int dir);
	void mouseCheck(unsigned int mouseX, unsigned int mouseY);
	int getAction();
	void Render(glm::vec2& center, const TextService& big, const TextService& action, const TextService& info);
	void Destroy();

	unsigned int pageID;
	std::vector<MenuPage*> pages;
};