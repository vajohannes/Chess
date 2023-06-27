#include "ChessPiece.h"

ChessPiece::ChessPiece(Player player, int row, int col, ChessPiece* (*chessBoard)[8][8]) :
    player(player), row(row), col(col), previousRow(row), previousCol(col), potentialCapturedChessPiece(nullptr), chessBoard(chessBoard) {}

int ChessPiece::getRow() const { return row; }
int ChessPiece::getCol() const { return col; }

bool ChessPiece::isEnemyChessPiece(const ChessPiece* const chessPiece) const {
	return (this->player != chessPiece->player);
}

void ChessPiece::calculatePotentialCaptures(bool potentialCaptures[8][8]) const {
	calculatePotentialMoves(potentialCaptures);
}

void ChessPiece::potentialMove(int row, int col) {
    potentialCapturedChessPiece = (*chessBoard)[row][col];
    previousRow = this->row;
    previousCol = this->col;
    (*chessBoard)[this->row][this->col] = nullptr;
    this->row = row;
    this->col = col;
    (*chessBoard)[row][col] = this;
}

void ChessPiece::undoPotentialMove() {
    (*chessBoard)[row][col] = potentialCapturedChessPiece;
    (*chessBoard)[previousRow][previousCol] = this;
    row = previousRow;
    col = previousCol;
}

ChessPiece* ChessPiece::move(int row, int col) {
    potentialMove(row, col);
    return potentialCapturedChessPiece;
}

void ChessPiece::calculateDirectionalMoveset(bool directions[9], bool calculatedMoveset[8][8]) const {
	// directions[9]: Down-Left, Down, Down-Right, Left, Center, Right, Up-Left, Up, Up-Right.
	// Check along specified directions in Breadth-First-Search style ring per outermost for-loop.
	for (int n = 1; n <= 7; n++) {
		int row = this->row - n;

		// Check each row in the ring.
		for (int i = 0; i < 3; i++, row += n) {
			int col = this->col - n;

			// Check each col in the ring.
			for (int j = 0; j < 3; j++, col += n) {
				if ((row < 0) || (row >= 8) || (col < 0) || (col >= 8)) continue;
				if ((row == this->row) && (col == this->col)) continue;
				if (!directions[i * 3 + j]) continue; // Ignore further checks along this direction if blocked by another ChessPiece.

				if ((*chessBoard)[row][col] != nullptr) {
					if (isEnemyChessPiece((*chessBoard)[row][col])) {
						calculatedMoveset[row][col] = true;
					}
					directions[i * 3 + j] = false; // Keep track that we have been blocked by another ChessPiece along this direction.
					continue;
				}
				calculatedMoveset[row][col] = true;
			}
		}
	}
}
