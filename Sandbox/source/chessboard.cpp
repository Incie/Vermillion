#include"pch/pch.h"
#include"chessboard.h"
#include"services.h"
#include"render.h"


ChessBoard::ChessBoard()
{
	GenerateBoard();
	SpawnBoardPieces();

	position = {150,50};
	clickedPiece.x = -1;
	clickedSquare.x = -1;
	teamToMove = ChessTeam::White;
	checkState = false;
}

ChessBoard::~ChessBoard()
{
	delete[]board;
	delete[]colors;

	pieces.clear();
}

void ChessBoard::SpawnBoardPieces()
{
	SpawnPiece({0,0}, ChessTeam::Black, ChessPieceType::Rook);
	SpawnPiece({7,0}, ChessTeam::Black, ChessPieceType::Rook);
	SpawnPiece({1,0}, ChessTeam::Black, ChessPieceType::Knight);
	SpawnPiece({6,0}, ChessTeam::Black, ChessPieceType::Knight);
	SpawnPiece({2,0}, ChessTeam::Black, ChessPieceType::Bishop);
	SpawnPiece({5,0}, ChessTeam::Black, ChessPieceType::Bishop);
	SpawnPiece({4,0}, ChessTeam::Black, ChessPieceType::King);
	SpawnPiece({3,0}, ChessTeam::Black, ChessPieceType::Queen);
	for(int i = 0; i < 8; ++i) {
		SpawnPiece({i,1}, ChessTeam::Black, ChessPieceType::Pawn);
	}

	SpawnPiece({0,7}, ChessTeam::White, ChessPieceType::Rook);
	SpawnPiece({7,7}, ChessTeam::White, ChessPieceType::Rook);
	SpawnPiece({1,7}, ChessTeam::White, ChessPieceType::Knight);
	SpawnPiece({6,7}, ChessTeam::White, ChessPieceType::Knight);
	SpawnPiece({2,7}, ChessTeam::White, ChessPieceType::Bishop);
	SpawnPiece({5,7}, ChessTeam::White, ChessPieceType::Bishop);
	SpawnPiece({4,7}, ChessTeam::White, ChessPieceType::King);
	SpawnPiece({3,7}, ChessTeam::White, ChessPieceType::Queen);
	for(int i = 0; i < 8; ++i) {
		SpawnPiece({i,6}, ChessTeam::White, ChessPieceType::Pawn);
	}
}

bool ChessBoard::HasSelection() 
{
	return (clickedSquare.x > -1);
}

void ChessBoard::IsInCheck()
{
	auto oldHighlight = possibleMoves;
	possibleMoves.clear();

	for(auto piece : pieces) {
		HighlightPieceMoves(piece);
	}

	checkState = false;
	for(auto pm : possibleMoves) {
		if(pm.isCheck) {
			checkState = true;
		}
	}

	possibleMoves.clear();
	possibleMoves = oldHighlight;
}

void ChessBoard::HighlightPieceMoves(const ChessPiece& piece)
{
	if(piece.type == ChessPieceType::Pawn) {
		auto direction = 1; //black 
		if(piece.team == ChessTeam::White)
			direction = -1;

		auto coord = glm::ivec2{piece.coords.x, piece.coords.y + 1 * direction};
		if(HighlightIfEmpty(coord) ) {
			auto coord2 = glm::ivec2{piece.coords.x, piece.coords.y + 2 * direction};

			constexpr int whitePawnStart = 6;
			constexpr int blackPawnStart = 1;
			if((piece.coords.y == whitePawnStart && piece.team == ChessTeam::White) || (piece.coords.y == blackPawnStart && piece.team == ChessTeam::Black)) {
				HighlightIfEmpty(coord2);
			}
		}

		HighlightIfNotEmpty(piece.coords + glm::ivec2{1, direction}, piece);
		HighlightIfNotEmpty(piece.coords + glm::ivec2{-1, direction}, piece);
		return;
	}

	if(piece.type == ChessPieceType::Knight) {
		std::vector<glm::ivec2> knightMoves{{2, 1}, {2,-1}, {-2,1}, {-2,-1}, {1,2}, {-1, 2}, {1, -2}, {-1,-2}};
		for(auto& knightMove : knightMoves) {
			auto boardKnightMove = piece.coords + knightMove;

			if(!IsInsideBounds(boardKnightMove))
				continue;

			if(!HighlightIfEmpty(boardKnightMove)) {
				HighlightIfNotEmpty(boardKnightMove, piece);
			}
		}

		return;
	}

	if(piece.type == ChessPieceType::King) {
		std::vector<glm::ivec2> kingMoves{{1,0},{-1,0},{1,1}, {0,1}, {-1,1}, {1,-1}, {0,-1}, {-1,-1}};
		for(auto& kingMove : kingMoves) {
			auto boardKingMove = piece.coords + kingMove;

			if(!IsInsideBounds(boardKingMove))
				continue;

			if(!HighlightIfEmpty(boardKingMove)) {
				HighlightIfNotEmpty(boardKingMove, piece);
			}
		}
		return;
	}

	int maxStride = 8;
	std::vector<glm::ivec2> directions;

	if(piece.type == ChessPieceType::Bishop) {
		directions.emplace_back(glm::ivec2{-1,-1});
		directions.emplace_back(glm::ivec2{ 1,-1});
		directions.emplace_back(glm::ivec2{ 1, 1});
		directions.emplace_back(glm::ivec2{-1, 1});
	} 
	if(piece.type == ChessPieceType::Queen) {
		directions.emplace_back(glm::ivec2{-1,-1});
		directions.emplace_back(glm::ivec2{1,-1});
		directions.emplace_back(glm::ivec2{1, 1});
		directions.emplace_back(glm::ivec2{-1, 1});
		directions.emplace_back(glm::ivec2{0,-1});
		directions.emplace_back(glm::ivec2{0, 1});
		directions.emplace_back(glm::ivec2{1, 0});
		directions.emplace_back(glm::ivec2{-1, 0});
	}
	if(piece.type == ChessPieceType::Rook) {
		directions.emplace_back(glm::ivec2{ 0,-1});
		directions.emplace_back(glm::ivec2{ 0, 1});
		directions.emplace_back(glm::ivec2{ 1, 0});
		directions.emplace_back(glm::ivec2{-1, 0});
	}

	for(auto& d : directions) {
		HighlightUntilHit(piece.coords, d, piece, maxStride);
	}
}

bool ChessBoard::HighlightIfEmpty(const glm::ivec2& coord)
{
	auto piece = GetPieceAt(coord);

	if(piece == nullptr) {
		auto pm = PossibleMove{coord, CenterOfTileAt(coord), glm::vec3(1,1,0), false, false};
		possibleMoves.emplace_back(pm);
		return true;
	}

	return false;
}

bool ChessBoard::HighlightIfNotEmpty(const glm::ivec2& coord, const ChessPiece& p)
{
	auto piece = GetPieceAt(coord);

	if(piece != nullptr && p.team != piece->team) {
		auto pm = PossibleMove{coord, CenterOfTileAt(coord), glm::vec3(1,0,0), false, false};

		if(piece->type == ChessPieceType::King)
			pm.isCheck = true;

		possibleMoves.emplace_back(pm);
		return true;
	}

	return false;
}

void ChessBoard::HighlightUntilHit(const glm::ivec2& fromCoord, const glm::ivec2& direction, const ChessPiece& piece, int maxLength)
{
	int strides = 1;
	auto coord = fromCoord + direction;
	while(this->InsideBoardBounds(coord)) {
		if(!HighlightIfEmpty(coord)) {
			HighlightIfNotEmpty(coord, piece);
			return;
		}

		coord += direction;
		strides++;
		if(strides == maxLength)
			return;
	}
}


void ChessBoard::Release(const glm::vec2& mousePosition)
{
	if(!HasSelection())
		return;

	auto boardCoords = ScreenToBoardCoordinates(mousePosition);

	bool found = false;
	for(auto& pm : possibleMoves) {
		if(pm.coords == boardCoords)
			found = true;
	}

	if(!found) {
		clickedSquare.x = -1;
		clickedPiece.x = -1;
		possibleMoves.clear();
		return;
	}

	auto capturedPiece = GetPieceAt(boardCoords);
	if(capturedPiece != nullptr) {
		auto removed = std::remove_if(pieces.begin(), pieces.end(), [&capturedPiece](const ChessPiece& p) { return p.coords == capturedPiece->coords; });
		pieces.erase(removed);
	}

	auto pieceToMove = GetPieceAt(clickedPiece);
	pieceToMove->coords = boardCoords;
	pieceToMove->position = glm::vec2(static_cast<float>(boardCoords.x) * tileSize, static_cast<float>(boardCoords.y) * tileSize) + glm::vec2(tileSize * 0.5f);

	clickedSquare.x = -1;
	clickedPiece.x = -1;
	possibleMoves.clear();

	teamToMove = (teamToMove == ChessTeam::White) ? ChessTeam::Black : ChessTeam::White;
}

void ChessBoard::Click(const glm::vec2& mousePosition)
{
	auto boardCoords = ScreenToBoardCoordinates(mousePosition);

	if(!InsideBoardBounds(boardCoords))
		return;

	if(HasSelection())
		return;

	auto piece = GetPieceAt(boardCoords);

	if(piece == nullptr)
		return;

	if(piece->team != teamToMove)
		return;

	clickedSquare = piece->position - 0.5f*tileSize + 1.0f;
	clickedPiece = piece->coords;
	hover.x = -1;

	HighlightPieceMoves(*piece);
}

void ChessBoard::Update(const glm::vec2& mousePosition)
{
	if(clickedSquare.x > -1)
		return;

	const glm::vec2& boardMousePosition = mousePosition - position;

	hover.x = -1;
	if(PointInBounds(boardMousePosition)) {
		auto coord = glm::floor(boardMousePosition / tileSize) * tileSize;

		if(!HasSelection()) {
			auto piece = GetPieceAt(ScreenToBoardCoordinates(mousePosition));

			if(piece != nullptr) {
				if(piece->team == teamToMove)
					hover = coord + glm::vec2(1.0f);
			}
		}
	}
}

void ChessBoard::Render(const TextService& chessText, const TextService& text)
{
	Render::PushMatrix();
		Render::Translate2D(position);

		constexpr auto vertexCount = 64 * 4;
		Render::Quads(board, colors, vertexCount);

		if(hover.x >= 0.0f) {
			Render::Quad(hover, glm::vec2(tileSize - 2.0f), Colors::Red);
		}

		if(clickedSquare.x >= 0.0f )
			Render::Quad(clickedSquare, glm::vec2(tileSize - 2.0f), Colors::Green);

		constexpr auto fontSize = 35;
		for(auto& p : pieces) {
			std::string s(1, static_cast<char>(p.type));
			chessText.Print(p.position.x, p.position.y, s, fontSize, p.color, true);
		}


		for(auto& m : possibleMoves) {
			Render::Circle(m.tileCenter, 5.0f, m.color);
		}
	Render::PopMatrix();

	text.Print(5.0, 30.0, fmt::format("{} to move", (teamToMove == ChessTeam::White ? "White" : "Black")), 20, Colors::White);
}

bool ChessBoard::IsInsideBounds(const glm::ivec2& boardPosition)
{
	if(boardPosition.x < 0 || boardPosition.y < 0)
		return false;

	if(boardPosition.x > 7 || boardPosition.y > 7)
		return false;

	return true;
}

glm::vec2 ChessBoard::CenterOfTileAt(const glm::ivec2& pos)
{
	return glm::vec2(pos.x*tileSize + tileSize*0.5f, pos.y * tileSize + tileSize * 0.5f);
}

ChessPiece* ChessBoard::GetPieceAt(const glm::ivec2& boardCoordinate)
{
	for(auto& piece : pieces) {
		if(piece.coords == boardCoordinate)
			return &piece;
	}

	return nullptr;
}

glm::ivec2 ChessBoard::ScreenToBoardCoordinates(const glm::vec2& screenCoords)
{
	auto fcoords = glm::floor( (screenCoords - position) / tileSize);
	return glm::ivec2(static_cast<int>(fcoords.x), static_cast<int>(fcoords.y));
}

void ChessBoard::GenerateBoard()
{
	constexpr int tileCount = 64;
	constexpr int vertexCount = tileCount * 4;
	board = vnew glm::vec2[vertexCount];
	colors = vnew glm::vec3[vertexCount];

	tileSize = 50.0f;

	//position = glm::vec2(50, 50);
	size = glm::vec2(8, 8) * 50.0f;

	glm::vec3 black = glm::vec3(0.4f);
	glm::vec3 white = glm::vec3(0.8f);

	glm::vec3 color = black;
	int colorCounter = 0;

	for(int y = 0; y < 8; ++y) {
		for(int x = 0; x < 8; ++x) {
			if(colorCounter % 2 == y % 2)
				color = black;
			else
				color = white;

			colorCounter++;

			int i = x * 4 + y * 4 * 8;
			float tX = x * tileSize;
			float tY = y * tileSize;
			board[i + 0] = glm::vec2(tX, tY);
			board[i + 1] = glm::vec2(tX + tileSize, tY);
			board[i + 2] = glm::vec2(tX + tileSize, tY + tileSize);
			board[i + 3] = glm::vec2(tX, tY + tileSize);

			colors[i + 0] = color;
			colors[i + 1] = color;
			colors[i + 2] = color;
			colors[i + 3] = color;
		}
	}
}

void ChessBoard::SpawnPiece(const glm::vec2& coordinate, ChessTeam team, ChessPieceType type)
{
	auto& piece = pieces.emplace_back(ChessPiece());

	piece.coords = coordinate;
	piece.team = team;
	piece.type = type;
	piece.position = {coordinate.x * 50 + 25, coordinate.y * 50 + 25};

	if(piece.team == ChessTeam::Black)
		piece.color = glm::vec3(0);
	if(piece.team == ChessTeam::White)
		piece.color = glm::vec3(1);
}

bool ChessBoard::InsideBoardBounds(const glm::ivec2& coords)
{
	if(coords.x < 0 || coords.y < 0)
		return false;

	if(coords.x > 7 || coords.y > 7)
		return false;

	return true;
}

bool ChessBoard::PointInBounds(const glm::vec2& coords)
{
	return (coords.x >= 0 && coords.y >= 0 && coords.x < size.x && coords.y < size.y);
}
