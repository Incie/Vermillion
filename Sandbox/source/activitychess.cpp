#include "activitychess.h"
#include"chessboard.h"

ActivityChess::ActivityChess()
{
}

ChessBoard chessboard;

void ActivityChess::Initialize()
{
}

void ActivityChess::Deinitialize()
{
}

void ActivityChess::Update(float delta)
{
}

void ActivityChess::Render()
{
	chessboard.Render(Services().Text());
}
