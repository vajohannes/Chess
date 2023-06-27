#include "ChessGame.h"

ChessGame::ChessGame() : game_window(nullptr),
                         chessBoard{},
                         whiteKing(new King(WHITE, 0, 4, &chessBoard)), blackKing(new King(BLACK, 7, 4, &chessBoard)),
                         pawnFirstMove(nullptr),
                         selectedChessPiece(nullptr), selectedChessPieceValidMoves{},
                         currentPlayer(WHITE)
{
    // White ChessPieces.
    chessBoard[0][0] = new Rook(WHITE, 0, 0, &chessBoard);
    chessBoard[0][1] = new Knight(WHITE, 0, 1, &chessBoard);
    chessBoard[0][2] = new Bishop(WHITE, 0, 2, &chessBoard);
    chessBoard[0][3] = new Queen(WHITE, 0, 3, &chessBoard);
    chessBoard[0][4] = whiteKing;
    chessBoard[0][5] = new Bishop(WHITE, 0, 5, &chessBoard);
    chessBoard[0][6] = new Knight(WHITE, 0, 6, &chessBoard);
    chessBoard[0][7] = new Rook(WHITE, 0, 7, &chessBoard);
    for (int n = 0; n < 8; n++)
    {
        chessBoard[1][n] = new Pawn(WHITE, 1, n, &chessBoard);
    }

    // Black ChessPieces;
    chessBoard[7][0] = new Rook(BLACK, 7, 0, &chessBoard);
    chessBoard[7][1] = new Knight(BLACK, 7, 1, &chessBoard);
    chessBoard[7][2] = new Bishop(BLACK, 7, 2, &chessBoard);
    chessBoard[7][3] = new Queen(BLACK, 7, 3, &chessBoard);
    chessBoard[7][4] = blackKing;
    chessBoard[7][5] = new Bishop(BLACK, 7, 5, &chessBoard);
    chessBoard[7][6] = new Knight(BLACK, 7, 6, &chessBoard);
    chessBoard[7][7] = new Rook(BLACK, 7, 7, &chessBoard);
    for (int n = 0; n < 8; n++)
    {
        chessBoard[6][n] = new Pawn(BLACK, 6, n, &chessBoard);
    }
}

ChessGame::~ChessGame()
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            delete chessBoard[i][j];
        }
    }
    delete game_window;
}

King *ChessGame::getPlayerKing(Player player) const
{
    return ((player == WHITE) ? whiteKing : blackKing);
}

bool ChessGame::hasValidMoves(Player player) const
{
    for (int row = 0; row < 8; row++)
    {
        for (int col = 0; col < 8; col++)
        {
            if ((chessBoard[row][col] != nullptr) && (chessBoard[row][col]->player == player))
            {
                bool playerChessPieceValidMoves[8][8] = {}; // Initialized to all false;
                calculateValidMoves(chessBoard[row][col], playerChessPieceValidMoves);
                for (int i = 0; i < 8; i++)
                {
                    for (int j = 0; j < 8; j++)
                    {
                        if (playerChessPieceValidMoves[i][j])
                            return true;
                    }
                }
            }
        }
    }
    return false;
}

void ChessGame::calculateValidMoves(ChessPiece *chessPiece, bool validMoves[8][8]) const
{
    fill_n(&validMoves[0][0], 64, false); // Reset all to false.

    if (chessPiece == nullptr)
        return;

    // First, get the potential moves.
    chessPiece->calculatePotentialMoves(validMoves);

    // Then, analyze each move if it will cause own king to be in-check.
    for (int row = 0; row < 8; row++)
    {
        for (int col = 0; col < 8; col++)
        {
            if (validMoves[row][col])
            {
                chessPiece->potentialMove(row, col);
                if (getPlayerKing(chessPiece->player)->calculatePotentialCheck())
                {
                    validMoves[row][col] = false;
                }
                chessPiece->undoPotentialMove();
            }
        }
    }
}

void ChessGame::promotePawn(Pawn *pawn)
{
    char choice = game_window->get_promotion_choice(currentPlayer);
    switch (choice)
    {
    case 'Q':
        chessBoard[pawn->getRow()][pawn->getCol()] = new Queen(pawn->player, pawn->getRow(), pawn->getCol(), &chessBoard);
        break;
    case 'R':
        chessBoard[pawn->getRow()][pawn->getCol()] = new Rook(pawn->player, pawn->getRow(), pawn->getCol(), &chessBoard);
        break;
    case 'N':
        chessBoard[pawn->getRow()][pawn->getCol()] = new Knight(pawn->player, pawn->getRow(), pawn->getCol(), &chessBoard);
        break;
    case 'B':
        chessBoard[pawn->getRow()][pawn->getCol()] = new Bishop(pawn->player, pawn->getRow(), pawn->getCol(), &chessBoard);
        break;
    default:
        break;
    }

    delete pawn;
}

void ChessGame::processUserInput(int row, int col)
{
    if ((selectedChessPiece == nullptr) || (selectedChessPiece->player != currentPlayer))
    {
        selectChessPiece(row, col);
    }
    else if (!selectedChessPieceValidMoves[row][col])
    {
        selectChessPiece(row, col);
    }
    else
    {
        moveSelectedChessPiece(row, col);
    }
}

void ChessGame::selectChessPiece(int row, int col)
{
    game_window->reset_highlighted();
    displayCurrentPlayerCheck();

    selectedChessPiece = chessBoard[row][col];
    calculateValidMoves(selectedChessPiece, selectedChessPieceValidMoves);

    if (selectedChessPiece == nullptr)
    {
        game_window->set_highlighted(row, col, true, "black");
    }
    else
    {
        game_window->set_highlighted(selectedChessPiece->getRow(), selectedChessPiece->getCol(), true, ((selectedChessPiece->player == currentPlayer) ? "green" : "purple"));
        displaySelectedChessPieceValidMoves();
    }
}

void ChessGame::moveSelectedChessPiece(int row, int col)
{
    game_window->set_piece(selectedChessPiece->getRow(), selectedChessPiece->getCol(), ' ');
    ChessPiece *occupyingChessPiece = selectedChessPiece->move(row, col);
    delete occupyingChessPiece;
    game_window->set_piece(row, col, selectedChessPiece->getImage());

    // If selectedChessPiece is a Pawn, process Pawn Promotion and En Passant if applicable.
    Pawn *pawn = dynamic_cast<Pawn *>(selectedChessPiece); // Will return nullptr if selectedChessPiece is not a Pawn.
    if (pawn != nullptr)
    {
        // First Move gives opportunity for Enemy En Passant for the next turn only.
        if (pawn->hasFirstMoved())
        {
            if (pawnFirstMove != nullptr)
            {
                pawnFirstMove->passTurnEnPassant();
            }
            pawnFirstMove = pawn;
        }

        // En Passant
        int rowEnPassant, colEnPassant;
        if (pawn->hasCapturedEnPassant(rowEnPassant, colEnPassant))
        {
            game_window->set_piece(rowEnPassant, colEnPassant, ' ');
        }

        // Pawn Promotion
        if (pawn->getRow() == ((pawn->player == WHITE) ? 7 : 0))
        {
            game_window->reset_highlighted();
            game_window->set_highlighted(pawn->getRow(), pawn->getCol(), true, "green");
            promotePawn(pawn);
            game_window->set_piece(row, col, chessBoard[row][col]->getImage());
        }
    }

    // If selectedChessPiece is a King, process Castling if applicable.
    if ((selectedChessPiece == getPlayerKing(currentPlayer)) && getPlayerKing(currentPlayer)->hasCastled())
    {
        updateCastlingGUI();
    }

    nextPlayerTurn();
}

void ChessGame::nextPlayerTurn()
{
    currentPlayer = ((currentPlayer == WHITE) ? BLACK : WHITE);
    selectedChessPiece = nullptr;
    fill_n(&selectedChessPieceValidMoves[0][0], 64, false); // Reset all to false.
    whiteKing->updateCheck();
    blackKing->updateCheck();

    // En Passant only valid for one turn.
    if (pawnFirstMove != nullptr)
    {
        if (pawnFirstMove->player == currentPlayer)
        {
            pawnFirstMove->passTurnEnPassant();
            pawnFirstMove = nullptr;
        }
    }

    game_window->reset_highlighted();
    game_window->set_current_player(currentPlayer);
    displayCurrentPlayerCheck();

    if (!hasValidMoves(currentPlayer))
    {
        if (getPlayerKing(currentPlayer)->isCheck())
        {
            game_window->show_checkmate_label();
        }
        else
        {
            game_window->show_stalemate_label();
        }
    }
}

GameWindow *ChessGame::get_game_window() const { return game_window; }

void ChessGame::startGraphicUI()
{
    game_window = new GameWindow(nullptr, this);
    for (int row = 0; row < 8; row++)
    {
        for (int col = 0; col < 8; col++)
        {
            connect(game_window->get_square(row, col), &Square::clicked_with_pos, this, &ChessGame::processUserInput);
            if (chessBoard[row][col] != nullptr)
            {
                game_window->set_piece(row, col, chessBoard[row][col]->getImage());
            }
        }
    }
    game_window->show();
}

void ChessGame::displaySelectedChessPieceValidMoves() const
{
    std::string color = ((selectedChessPiece->player == currentPlayer) ? "green" : "purple");
    for (int row = 0; row < 8; row++)
    {
        for (int col = 0; col < 8; col++)
        {
            if (selectedChessPieceValidMoves[row][col])
            {
                game_window->set_highlighted(row, col, true, color);
            }
        }
    }
}

void ChessGame::displayCurrentPlayerCheck() const
{
    const King *const king = getPlayerKing(currentPlayer);
    if (king->isCheck())
    {
        game_window->set_highlighted(king->getRow(), king->getCol(), true, "red");
    }
}

void ChessGame::updateCastlingGUI() const
{
    const King *const king = getPlayerKing(currentPlayer);
    if (king->hasCastledKingSide())
    {
        game_window->set_piece(king->getRow(), 7, ' ');
        game_window->set_piece(king->getRow(), 5, ((king->player == WHITE) ? 'R' : 'r'));
    }
    else if (king->hasCastledQueenSide())
    {
        game_window->set_piece(king->getRow(), 0, ' ');
        game_window->set_piece(king->getRow(), 3, ((king->player == WHITE) ? 'R' : 'r'));
    }
}

void ChessGame::startConsoleUI()
{
    do
    {
        displayChessBoardConsole();
        cout << ((currentPlayer == WHITE) ? "White" : "Black") << "'s Turn." << endl;

        int row, col;
        char confirm;
        do
        {
            do
            {
                do
                {
                    cout << "Select your Chess Piece (col, row): ";
                    cin >> col >> row;
                    col -= 1;
                    row -= 1;
                    if ((row < 0) || (row >= 8) || (col < 0) || (col >= 8))
                        continue;

                    if ((chessBoard[row][col] != nullptr) && (chessBoard[row][col]->player != currentPlayer))
                    {
                        selectedChessPiece = nullptr;
                    }
                    else
                    {
                        selectedChessPiece = chessBoard[row][col];
                    }
                    calculateValidMoves(selectedChessPiece, selectedChessPieceValidMoves);
                } while (selectedChessPiece == nullptr);
                cout << endl;
                displaySelectedChessPieceValidMovesConsole();

                cout << "Confirm selection? (Y/N) ";
                cin >> confirm;
            } while ((confirm != 'Y') && (confirm != 'y'));

            cout << "Make your Move (col, row): ";
            cin >> col >> row;
            col -= 1;
            row -= 1;
            cout << endl;
        } while ((row < 0) || (row >= 8) || (col < 0) || (col >= 8) || !selectedChessPieceValidMoves[row][col]);
        ChessPiece *occupyingChessPiece = selectedChessPiece->move(row, col);
        delete occupyingChessPiece;

        selectedChessPiece = nullptr;
        fill_n(&selectedChessPieceValidMoves[0][0], 64, false); // Reset all to false.
        currentPlayer = ((currentPlayer == WHITE) ? BLACK : WHITE);
        getPlayerKing(currentPlayer)->updateCheck();
    } while (hasValidMoves(currentPlayer));

    displayChessBoardConsole();
    if (getPlayerKing(currentPlayer)->isCheck())
    {
        cout << ((currentPlayer == WHITE) ? "White" : "Black") << " Checkmate! " << ((currentPlayer == WHITE) ? "Black" : "White") << " wins!" << endl;
    }
    else
    {
        cout << "Stalemate!" << endl;
    }
}

void ChessGame::displayChessBoardConsole() const
{
    cout << " |---|---|---|---|---|---|---|---|\n";
    for (int row = 7; row >= 0; row--)
    {
        cout << row + 1;
        for (int col = 0; col < 8; col++)
        {
            cout << "|";

            if (chessBoard[row][col] == nullptr)
            {
                cout << "   ";
            }
            else
            {
                if ((chessBoard[row][col] == getPlayerKing(currentPlayer)) && getPlayerKing(currentPlayer)->isCheck())
                {
                    cout << "!" << chessBoard[row][col]->getImage() << "!";
                }
                else
                {
                    cout << " " << chessBoard[row][col]->getImage() << " ";
                }
            }
        }
        cout << "|\n";
        cout << " |---|---|---|---|---|---|---|---|\n";
    }
    cout << " | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |\n";
    cout << endl;
}

void ChessGame::displaySelectedChessPieceValidMovesConsole() const
{
    cout << " |---|---|---|---|---|---|---|---|\n";
    for (int row = 7; row >= 0; row--)
    {
        cout << row + 1;
        for (int col = 0; col < 8; col++)
        {
            cout << "|";
            if (chessBoard[row][col] == nullptr)
            {
                if (selectedChessPieceValidMoves[row][col])
                {
                    cout << "< >";
                }
                else
                {
                    cout << "   ";
                }
            }
            else
            {
                if (selectedChessPieceValidMoves[row][col] || ((row == selectedChessPiece->getRow()) && (col == selectedChessPiece->getCol())))
                {
                    cout << "<" << chessBoard[row][col]->getImage() << ">";
                }
                else if ((chessBoard[row][col] == getPlayerKing(currentPlayer)) && getPlayerKing(currentPlayer)->isCheck())
                {
                    cout << "!" << chessBoard[row][col]->getImage() << "!";
                }
                else
                {
                    cout << " " << chessBoard[row][col]->getImage() << " ";
                }
            }
        }
        cout << "|\n";
        cout << " |---|---|---|---|---|---|---|---|\n";
    }
    cout << " | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |\n";
    cout << endl;
}
