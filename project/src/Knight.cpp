#include "Knight.h"

Knight::Knight(Player player, int row, int col, ChessPiece* (*chessBoard)[8][8]) :
	ChessPiece(player, row, col, chessBoard) {}

char Knight::getImage() const { return ((player == WHITE)? IMAGE_WHITE : IMAGE_BLACK); }

void Knight::calculatePotentialMoves(bool potentialMoves[8][8]) const {
    for (int i = -1; i <= 1; i += 2) {
        for (int j = -1; j <= 1; j += 2) {
            for (int k = 0; k < 2; k++) {
                int row = this->row + i + k*i;
                int col = this->col + j + (1-k)*j;
				if ((row < 0) || (row >= 8) || (col < 0) || (col >= 8)) continue;

				if ((*chessBoard)[row][col] == nullptr) {
					potentialMoves[row][col] = true;
				}
				else {
					if (isEnemyChessPiece((*chessBoard)[row][col])) {
						potentialMoves[row][col] = true;
					}
				}
			}
		}
	}
}
