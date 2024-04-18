#ifndef CHESS_H
#define CHESS_H
#include "kirous.h"
#include <stdlib.h>
#include <vector>
#include <memory>

#define BOARD_SIZE 8
#define BACKROW 0
#define FRONTROW 1
#define EMPTYROW 2
#define RED 1
#define BLUE 3

class chessPiece {
    protected:
        int moveEmptySpace(CursorLoc &newLoc, CursorLoc &oldLoc, std::vector<std::vector<std::unique_ptr<chessPiece>>>& board);
        int moveOccupiedSpace(CursorLoc &newLoc, CursorLoc &oldLoc, std::vector<std::vector<std::unique_ptr<chessPiece>>>& board);
        bool calcPath(CursorLoc &newLoc, CursorLoc &oldLoc, std::vector<std::vector<std::unique_ptr<chessPiece>>>& board);
    public:
        virtual int move(CursorLoc &newLoc, CursorLoc &oldLoc, std::vector<std::vector<std::unique_ptr<chessPiece>>>& board) = 0;
        char chessChar = ' '; //Chess piece
        int pieceColour; //Player colour
        bool isEmpty;
        virtual ~chessPiece() = default;
};

class pawnPiece : public chessPiece {
    public: 
        pawnPiece() {
            chessChar = 'S';
            isEmpty = false;
        };
        int move(CursorLoc &newLoc, CursorLoc &oldLoc, std::vector<std::vector<std::unique_ptr<chessPiece>>>& board) override;
        bool firstMove = true;
};

class towerPiece : public chessPiece {
    public: 
        towerPiece() {
            chessChar = 'T';
            isEmpty = false;
        };
        int move(CursorLoc &newLoc, CursorLoc &oldLoc, std::vector<std::vector<std::unique_ptr<chessPiece>>>& board) override;
};

class bishopPiece : public chessPiece {
    public: 
        bishopPiece() {
            chessChar = 'B';
            isEmpty = false;
        };
        int move(CursorLoc &newLoc, CursorLoc &oldLoc, std::vector<std::vector<std::unique_ptr<chessPiece>>>& board) override;
};

class queenPiece : public chessPiece {
    public: 
        queenPiece() {
            chessChar = 'Q';
            isEmpty = false;
        };
        int move(CursorLoc &newLoc, CursorLoc &oldLoc, std::vector<std::vector<std::unique_ptr<chessPiece>>>& board) override;
};

class horsePiece : public chessPiece {
    public: 
        horsePiece() {
            chessChar = 'H';
            isEmpty = false;
        };
        int move(CursorLoc &newLoc, CursorLoc &oldLoc, std::vector<std::vector<std::unique_ptr<chessPiece>>>& board) override;
};

class kingPiece : public chessPiece {
    public: 
        kingPiece() {
            chessChar = 'K';
            isEmpty = false;
        };
        int move(CursorLoc &newLoc, CursorLoc &oldLoc, std::vector<std::vector<std::unique_ptr<chessPiece>>>& board) override;
};

class ChessBoard {
    private:
        int turnCount = 0;
        void FillRow(int row, int& unit_colour, std::vector<std::unique_ptr<chessPiece>>&Board);
    public:
        ChessBoard();
        std::vector<std::vector<std::unique_ptr<chessPiece>>> board;
};

#endif