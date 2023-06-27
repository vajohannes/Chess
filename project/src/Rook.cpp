#include "Rook.h"

Rook::Rook(Player player, int row, int col, ChessPiece* (*chessBoard)[8][8]) :
    ChessPiece(player, row, col, chessBoard), hasMoved(false) {}

char Rook::getImage() const { return ((player == WHITE)? IMAGE_WHITE : IMAGE_BLACK); }

void Rook::calculatePotentialMoves(bool potentialMoves[8][8]) const {
	// !Down-Left, Down, !Down-Right, Left, !Center, Right, !Up-Left, Up, !Up-Right.
	bool directions[9] = {false, true, false, true, false, true, false, true, false};
	calculateDirectionalMoveset(directions, potentialMoves);
}

ChessPiece* Rook::move(int row, int col) {
    hasMoved = true;
    return ChessPiece::move(row, col);
}

bool Rook::canCastle() const { return !hasMoved; }
