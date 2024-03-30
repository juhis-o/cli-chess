#ifndef CHESS_H
#define CHESS_H
#include "kirous.h"
#define BOARD_SIZE 8

class chessPiece {
    public:
        char chessChar = ' '; //Chess piece
        bool bgColour; //Chessboard Cellcolour
	    int pieceColour; //Player colour
};

class ChessBoard {
    private:
        void FillRow(char* row, bool& bgColour, int& unit_colour, chessPiece *Board);

    public:
        ChessBoard();
        void MovePiece(CursorLoc &newLoc, CursorLoc &oldLoc);
        chessPiece board[BOARD_SIZE][BOARD_SIZE];
};

#endif