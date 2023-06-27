#include "Bishop.h"

Bishop::Bishop(Player player, int row, int col, ChessPiece* (*chessBoard)[8][8]) :
	ChessPiece(player, row, col, chessBoard) {}

char Bishop::getImage() const { return ((player == WHITE)? IMAGE_WHITE : IMAGE_BLACK); }

void Bishop::calculatePotentialMoves(bool potentialMoves[8][8]) const {
	// Down-Left, !Down, Down-Right, !Left, !Center, !Right, Up-Left, !Up, Up-Right.
	bool directions[9] = {true, false, true, false, false, false, true, false, true};
	calculateDirectionalMoveset(directions, potentialMoves);
}
