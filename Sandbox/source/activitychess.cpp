#include "activitychess.h"


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

	chessboard.Update(Services().Input().GetMousePosition());
}

void ActivityChess::Render()
{
	chessboard.Render(Services().Text());
}
