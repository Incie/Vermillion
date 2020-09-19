#include"pch/pch.h"
#include"Menu.h"

Menu::Menu()
{
	pageID = 0;
}

Menu::~Menu()
{
}

void Menu::addMenuPage(MenuPage* p)
{
	pages.push_back(p);
}

int Menu::getAction()
{
	return pages[pageID]->getAction();
}

void Menu::moveSelection(int dir)
{
	pages[pageID]->movePointer(dir);
}

void Menu::mouseCheck(unsigned int mouseX, unsigned int mouseY)
{
}

void Menu::Render(glm::vec2& center, const TextService& big, const TextService& action, const TextService& info)
{
	pages[pageID]->Render(center, big, action, info);
}

void Menu::Destroy()
{
	for(unsigned int i = 0; i < pages.size(); ++i)
	{
		pages[i]->Destroy();
		delete pages[i];
	}

	pages.clear();
}