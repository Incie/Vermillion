#include"pch/pch.h"
#include"Menu.h"
#include"GL/glew.h"
#include"services.h"

MenuPage::MenuPage()
{
}

MenuPage::~MenuPage()
{
}


void MenuPage::movePointer(int dir)
{
	selection += dir;

	if(selection >= (int)actions.size())
		selection = 0;

	if(selection < 0)
		selection = (int)actions.size() - 1;
}

void MenuPage::mouseCheck(unsigned int mouseX, unsigned int mouseY)
{
	//todo
}

int MenuPage::getAction()
{
	return actions[selection].ID;
}

//void MenuPage::addElement( MenuAction *a )?
void MenuPage::addElement(std::string title, std::string info, unsigned int ID, bool bActive)
{
	MenuAction action;
	action.title = title;
	action.info = info;
	action.ID = ID;
	action.active = bActive;

	actions.push_back(action);
}

void MenuPage::setActive(unsigned int actionID, bool bActive)
{
	for(unsigned int i = 0; i < actions.size(); ++i)
	{
		if(actions[i].ID == actionID)
		{
			actions[i].active = bActive;
			return;
		}
	}
}

void MenuPage::Render(glm::vec2& center, const TextService& big, const TextService& action, const TextService& info)
{
	float screenPosX = center.x;
	float screenPosY = 25;
	float offsetY = 50.0f;
	constexpr unsigned int fontBig = 100;
	constexpr unsigned int fontAction = 35;
	constexpr unsigned int fontSmall = 20;
	big.Print(screenPosX, screenPosY, title, 36, glm::vec3(0, 0.6f, 0), true, false);


	offsetY += 100.0f;
	for(unsigned int i = 0; i < actions.size(); ++i)
	{
		MenuAction& a = actions[i];

		glColor3f(0.5f, 0.5f, 0.5f);
		if(selection == i)
			glColor3f(1, 1, 1);

		action.Print(screenPosX, static_cast<double>(screenPosY + offsetY), a.title, fontAction, glm::vec3(0.5), true, false);
		offsetY += 35.0f;

		if(selection == i)
		{
			glm::vec3 color = a.active ? Colors::Green : Colors::Red;
			offsetY -= 25.0f * 0.5f;
			info.Print(screenPosX, static_cast<double>(screenPosY + offsetY), a.info, fontSmall, color, true, false);
			offsetY += 25.0f;
			glColor3f(1, 1, 1);
		}
	}
}

void MenuPage::Destroy()
{
	actions.clear();
}