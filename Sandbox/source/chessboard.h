#pragma once

#include"glm/glm.hpp"
#include<memory>
#include<vector>

enum class ChessPieceType {
	  King = 'k',
	 Queen = 'q',
	  Rook = 'r',
	Bishop = 'b',
	Knight = 'n',
	  Pawn = 'p',
};

enum class ChessTeam {
	White = 0,
	Black = 1
};

struct ChessPiece {
	glm::ivec2 coords;
	glm::vec2 position;
	glm::vec3 color;
	ChessPieceType type;
	ChessTeam team;
};

class TextService;

class ChessBoard {
public:
	ChessBoard();
	~ChessBoard();

	void Render(const TextService& text);
private:
	void SpawnPiece(const glm::vec2& coordinate, ChessTeam team, ChessPieceType type);

	std::vector<ChessPiece> pieces;
	glm::vec2* board;
	glm::vec3* colors;
};