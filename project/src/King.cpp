#include "King.h"

King::King(Player player, int row, int col, ChessPiece* (*chessBoard)[8][8]) :
    ChessPiece(player, row, col, chessBoard), check(false), hasMoved(false), castledKingSide(false), castledQueenSide(false) {}

char King::getImage() const { return ((player == WHITE)? IMAGE_WHITE : IMAGE_BLACK); }

void King::calculatePotentialMoves(bool potentialMoves[8][8]) const {
    calculatePotentialCaptures(potentialMoves); // Prevents recursion with Castling.

    // Castling King Side.
    if (canCastleGivenSide(KING_SIDE)) {
        potentialMoves[row][6] = true;
    }
    // Castling Queen Side.
    if (canCastleGivenSide(QUEEN_SIDE)) {
        potentialMoves[row][2] = true;
    }
}

void King::calculatePotentialCaptures(bool potentialCaptures[8][8]) const {
    // Regular Movement. King can't capture with Castling.
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if ((i == 0) && (j == 0)) continue;

            int row = this->row + i;
            int col = this->col + j;
            if ((row < 0) || (row >= 8) || (col < 0) || (col >= 8)) continue;

            if ((*chessBoard)[row][col] == nullptr) {
                    potentialCaptures[row][col] = true;
            }
            else {
                if (isEnemyChessPiece((*chessBoard)[row][col])) {
                    potentialCaptures[row][col] = true;
                }
            }
        }
    }
}

ChessPiece* King::move(int row, int col) {
    hasMoved = true;
    castledKingSide = castledQueenSide = false;

    // We have already processed all the Castling Requirements before we arrive here, so we just need to check if King has moved 2 steps and which side.
    // Castling King Side.
    if ((col - this->col) == 2) {
        (*chessBoard)[this->row][7]->move(this->row, 5);
        castledKingSide = true;
    }
    // Castling Queen Side.
    if ((col - this->col) == -2) {
        (*chessBoard)[this->row][0]->move(this->row, 3);
        castledQueenSide = true;
    }

    return ChessPiece::move(row, col);
}

bool King::calculatePotentialCheck() const {
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            if (((*chessBoard)[row][col] != nullptr) && (isEnemyChessPiece((*chessBoard)[row][col]))) {
                bool enemyChessPiecePotentialCaptures[8][8] = {}; // Initialized to all false;
                (*chessBoard)[row][col]->calculatePotentialCaptures(enemyChessPiecePotentialCaptures);
                if (enemyChessPiecePotentialCaptures[this->row][this->col]) return true;
            }
        }
    }
    return false;
}

void King::updateCheck() { check = calculatePotentialCheck(); }
bool King::isCheck() const { return check; }

bool King::hasCastled() const { return (hasCastledKingSide() || hasCastledQueenSide()); }
bool King::hasCastledKingSide() const { return castledKingSide; }
bool King::hasCastledQueenSide() const { return castledQueenSide; }

bool King::canCastleGivenSide(CastlingSide castlingSide) const {
    if (hasMoved) return false;
    if (isCheck()) return false;
    if (((*chessBoard)[row][((castlingSide == KING_SIDE)? 5 : 3)] != nullptr) || ((*chessBoard)[row][((castlingSide == KING_SIDE)? 6 : 2)] != nullptr)) return false;
    if ((castlingSide == QUEEN_SIDE) && ((*chessBoard)[row][1] != nullptr)) return false;

    Rook* rook = dynamic_cast<Rook*>((*chessBoard)[row][((castlingSide == KING_SIDE)? 7 : 0)]);
    if ((rook == nullptr) || !(rook->canCastle())) return false;

    const_cast<King*>(this)->potentialMove(row, ((castlingSide == KING_SIDE)? 5 : 3));
    bool potentialCheckFirstSquare = calculatePotentialCheck();
    const_cast<King*>(this)->undoPotentialMove();
    if (potentialCheckFirstSquare) return false;

    const_cast<King*>(this)->potentialMove(row, ((castlingSide == KING_SIDE)? 6 : 2));
    bool potentialCheckSecondSquare = calculatePotentialCheck();
    const_cast<King*>(this)->undoPotentialMove();
    if (potentialCheckSecondSquare) return false;

    return true;
}
