#include"pch/pch.h"
#include"menutest.h"
#include"ui/uiimage.h"
#include"ui/uibutton.h"
#include"ui/uitext.h"
#include"ui/uiview.h"
#include"ui/uiview_vertical.h"


namespace UI {
#define COLOR(name, r,g,b) glm::vec3 name = glm::vec3(r/255.0f,g/255.0f,b/255.0f)

	COLOR(Dark, 19, 15, 64);
	COLOR(Light, 223, 249, 251);
	COLOR(Positive, 106, 176, 76);
	COLOR(Negative, 235, 77, 75);
	COLOR(Neutral, 72, 52, 212);
};

MenuTest::MenuTest()
{
	TRACE("~MenuTest");
}

void MenuTest::Initialize()
{
	auto vw = new VerticalWindow([this](int id) {
		if(id == 1)
		{
			StartActivity("ActivityChess");
			return;
		}

		if(id == 9) {
			Finish();
			return;
		}
	});
	vw->BackgroundColor(UI::Dark);
	vw->SetPosition(glm::vec2(50, 100));

	auto title = new UIText("Game Title Here");
	title->Color(UI::Light);
	title->FontHeight(46.0f);
	title->SetSize(100, 26);
	title->SetMargin(4.0f, 8.0f);
	vw->AddChild(title);

	auto buttonStart = new UIButton("Start", 26, 1);
	buttonStart->SetSize(100, 26);
	buttonStart->SetColor(UI::Light);
	buttonStart->SetTextColor(UI::Dark);
	buttonStart->SetMargin(0, 2.0f);
	vw->AddChild(buttonStart);

	auto buttonExit = new UIButton("Exit", 26, 9);
	buttonExit->SetSize(100, 26);
	buttonExit->SetColor(UI::Negative);
	buttonExit->SetTextColor(UI::Dark);
	vw->AddChild(buttonExit);

	AddView(vw, 1);
}

void MenuTest::Deinitialize()
{
	DeinitializeUI();
}

void MenuTest::Update(float delta)
{
	UpdateUI(delta);
}

void MenuTest::Render()
{
	std::string pawns = "pppppppp";
	std::string notpawns = "rnbqkbnr";

	std::string pawns2 = "oooooooo";
	std::string notpawns2 = "tmvwlvmt";

	auto& text = Services().Text();
	auto& chessText = Services().Text(1);
	chessText.Print(25, 440, notpawns2, 30, Colors::White);
	chessText.Print(25, 470, pawns2, 30, Colors::White);

	chessText.Print(25, 500, pawns, 30, Colors::White);
	chessText.Print(25, 530, notpawns, 30, Colors::White);
	text.Print(25, 400, "VERMILLION", 26, glm::vec3(1, 1, 1));

	RenderUI();
}
