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

struct PossibleMove {
	glm::ivec2 coords;
	glm::vec2 tileCenter;
	glm::vec3 color;
	bool isCapture;
};

class ChessBoard {
public:
	ChessBoard();
	~ChessBoard();

	void Release(const glm::vec2& mousePosition);
	void Click(const glm::vec2& mousePosition);
	void Update(const glm::vec2& mousePosition);
	void Render(const TextService& text);
private:
	bool HasSelection();
	void HighlightPieceMoves(const ChessPiece& piece);
	bool HighlightIfEmpty(const glm::ivec2& coord);
	bool HighlightIfNotEmpty(const glm::ivec2& coord, const ChessPiece& piece);
	void HighlightUntilHit(const glm::ivec2& fromCoord, const glm::ivec2& direction, const ChessPiece& piece, int maxLength = 8);


	bool ValidMove(const glm::ivec2& boardPosition);
	glm::vec2 CenterOfTileAt(const glm::ivec2& pos);
	ChessPiece* GetPieceAt(const glm::ivec2& boardCoordinate);
	glm::ivec2 ScreenToBoardCoordinates(const glm::vec2& screen);
	void GenerateBoard();
	void SpawnPiece(const glm::vec2& coordinate, ChessTeam team, ChessPieceType type);
	bool InsideBoardBounds(const glm::ivec2& coords);
	bool PointInBounds(const glm::vec2& coords);

	std::vector<ChessPiece> pieces;
	std::vector<PossibleMove> possibleMoves;

	glm::ivec2 clickedPiece;
	glm::vec2 clickedSquare;

	glm::vec2 hover;
	float tileSize;
	glm::vec2 position;
	glm::vec2 size;
	glm::vec2* board;
	glm::vec3* colors;
};