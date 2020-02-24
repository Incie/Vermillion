#include"pch/pch.h"
#include"menutest.h"

MenuTest::MenuTest()
{
}

void MenuTest::Initialize()
{
}

void MenuTest::Deinitialize()
{
}

void MenuTest::Update(float delta)
{
}

void MenuTest::Render()
{
	Services().Text().Print(0, 20, "MenuTest!", 55, Colors::Green);
}
