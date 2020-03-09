#include"pch/pch.h"
#include"chessboard.h"
#include"services.h"
#include"render.h"

ChessBoard::ChessBoard()
{
	board = new glm::vec2[64 * 4];
	colors = new glm::vec3[64 * 4];

	float tileSize = 50.0f;

	glm::vec3 black = glm::vec3(0.2f), 
			  white = glm::vec3(0.8f);
	glm::vec3 color = black;
	int colorCounter = 0;

	for(int y = 0; y < 8; ++y) {
		for(int x = 0; x < 8; ++x) {
			if(colorCounter % 2 == y % 2)
				color = black;
			else color = white;
			colorCounter++;

			int i = x * 4 + y * 4 * 8;
			float tX = x * tileSize;
			float tY = y * tileSize;
			board[i+0] = glm::vec2(tX, tY);
			board[i+1] = glm::vec2(tX+tileSize, tY);
			board[i+2] = glm::vec2(tX + tileSize, tY + tileSize);
			board[i+3] = glm::vec2(tX, tY + tileSize);

			colors[i+0] = color;
			colors[i+1] = color;
			colors[i+2] = color;
			colors[i+3] = color;
		}
	}

	SpawnPiece({0,0}, ChessTeam::Black, ChessPieceType::Rook);
	SpawnPiece({7,0}, ChessTeam::Black, ChessPieceType::Rook);
	SpawnPiece({1,0}, ChessTeam::Black, ChessPieceType::Knight);
	SpawnPiece({6,0}, ChessTeam::Black, ChessPieceType::Knight);
	SpawnPiece({2,0}, ChessTeam::Black, ChessPieceType::Bishop);
	SpawnPiece({5,0}, ChessTeam::Black, ChessPieceType::Bishop);
	SpawnPiece({3,0}, ChessTeam::Black, ChessPieceType::King);
	SpawnPiece({4,0}, ChessTeam::Black, ChessPieceType::Queen);
	for(int i = 0; i < 8; ++i) {
		SpawnPiece({i,1}, ChessTeam::Black, ChessPieceType::Pawn);
	}

	SpawnPiece({0,7}, ChessTeam::White, ChessPieceType::Rook);
	SpawnPiece({7,7}, ChessTeam::White, ChessPieceType::Rook);
	SpawnPiece({1,7}, ChessTeam::White, ChessPieceType::Knight);
	SpawnPiece({6,7}, ChessTeam::White, ChessPieceType::Knight);
	SpawnPiece({2,7}, ChessTeam::White, ChessPieceType::Bishop);
	SpawnPiece({5,7}, ChessTeam::White, ChessPieceType::Bishop);
	SpawnPiece({3,7}, ChessTeam::White, ChessPieceType::King);
	SpawnPiece({4,7}, ChessTeam::White, ChessPieceType::Queen);
	for(int i = 0; i < 8; ++i) {
		SpawnPiece({i,6}, ChessTeam::White, ChessPieceType::Pawn);
	}
}

ChessBoard::~ChessBoard()
{
	delete[]board;
	delete[]colors;

	pieces.clear();
}


void ChessBoard::Render(const TextService& text)
{
	Render::Quads(board, colors, 64 * 4);

	for(auto& p : pieces) {
		std::string s(1, (unsigned char)p.type);
		text.Print(p.position.x, p.position.y, s, 35, p.color, true);
	}
}

void ChessBoard::SpawnPiece(const glm::vec2& coordinate, ChessTeam team, ChessPieceType type)
{
	pieces.push_back(ChessPiece());
	auto& piece = pieces.back();


	piece.coords = coordinate;
	piece.team = team;
	piece.type = type;
	piece.position = {coordinate.x * 50 + 25, coordinate.y * 50 + 25};

	if(piece.team == ChessTeam::Black)
		piece.color = glm::vec3(0);
	if(piece.team == ChessTeam::White)
		piece.color = glm::vec3(1);
}
