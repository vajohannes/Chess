#include "Pawn.h"

Pawn::Pawn(Player player, int row, int col, ChessPiece* (*chessBoard)[8][8]) :
    ChessPiece(player, row, col, chessBoard), hasMoved(false), firstMoved(false), capturedEnPassant(false), rowEnPassant(-1), colEnPassant(-1) {}

char Pawn::getImage() const { return ((player == WHITE)? IMAGE_WHITE : IMAGE_BLACK); }

void Pawn::calculatePotentialMoves(bool potentialMoves[8][8]) const {
	calculatePotentialCaptures(potentialMoves);

    int row = this->row + ((player == WHITE)? 1 : -1);
    if ((row < 0) || (row >= 8)) return;

    if ((*chessBoard)[row][this->col] == nullptr) {
        potentialMoves[row][this->col] = true;

        // First Move, 2 spaces in front.
        if (!hasMoved) {
            row += ((player == WHITE)? 1 : -1);
            if ((*chessBoard)[row][this->col] == nullptr) {
                potentialMoves[row][this->col] = true;
            }
        }
    }
}

void Pawn::calculatePotentialCaptures(bool potentialCaptures[8][8]) const {
	for (int i = -1; i <= 1; i+= 2) {
        int row = this->row + ((player == WHITE)? 1 : -1);
		int col = this->col + i;
        if ((col < 0) || (col >= 8)) continue;

        // En Passant.
        Pawn* pawn = dynamic_cast<Pawn*>((*chessBoard)[this->row][col]);
        if ((pawn != nullptr) && (isEnemyChessPiece(pawn)) && (pawn->hasFirstMoved())) {
            potentialCaptures[row][col] = true;
        }

        // Regular Capture.
        if ((row < 0) || (row >=8)) continue;
        if (((*chessBoard)[row][col] != nullptr) && (isEnemyChessPiece((*chessBoard)[row][col]))) {
			potentialCaptures[row][col] = true;
		}
	}
}

ChessPiece* Pawn::move(int row, int col) {
    hasMoved = true;
    firstMoved = (row == (this->row + ((player == WHITE)? 2 : -2)));

    // Changing col means capture, and empty occupyingChessPiece only happens during En Passant.
    int previousCol = this->col;
    ChessPiece* occupyingChessPiece = ChessPiece::move(row, col);
    if ((occupyingChessPiece == nullptr) && col != previousCol) {
        rowEnPassant = row - ((player == WHITE)? 1 : -1);
        colEnPassant = col;
        capturedEnPassant = true;
        occupyingChessPiece = (*chessBoard)[rowEnPassant][colEnPassant];
        (*chessBoard)[rowEnPassant][colEnPassant] = nullptr;
    }
    else {
        capturedEnPassant = false;
    }

    return occupyingChessPiece;
}

bool Pawn::hasFirstMoved() const { return firstMoved; }

bool Pawn::hasCapturedEnPassant(int& rowEnPassant, int& colEnPassant) const {
    rowEnPassant = this->rowEnPassant;
    colEnPassant = this->colEnPassant;
    return capturedEnPassant;
}

void Pawn::passTurnEnPassant() { firstMoved = false; }
