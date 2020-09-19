#include "activitychess.h"


//check detection
//check mate detection
//check detection after move + prevent piece move
//alternate team moves
//random ai -- rand(piece) + rand(possibleMove)
//ui for moves, timers, captures


ActivityChess::ActivityChess()
{
}

void ActivityChess::Initialize()
{
}

void ActivityChess::Deinitialize()
{
}

void ActivityChess::Update(float delta)
{
	auto& input = Services().Input();
	if(input.KeyOnce(VKey_LBUTTON))
		chessboard.Click(input.GetMousePosition());
	if(input.KeyUp(VKey_LBUTTON))
		chessboard.Release(input.GetMousePosition());

	if(input.KeyOnce(VKey_ESCAPE))
		StartActivity("MenuTest");

	chessboard.Update(input.GetMousePosition());
}

void ActivityChess::Render()
{
	chessboard.Render(Services().Text(1), Services().Text(0));
}

void ActivityChess::Resize(const glm::ivec2& newWindowSize)
{
	Activity::Resize(newWindowSize);

	//resize the chessboard and UI
}
