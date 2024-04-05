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
#define RED 0
#define BLUE 3

class chessPiece {
    public:
        virtual int move(CursorLoc &newLoc, CursorLoc &oldLoc, std::vector<std::vector<std::unique_ptr<chessPiece>>>& board) = 0;
        void moveEmptySpace(CursorLoc &newLoc, CursorLoc &oldLoc, std::vector<std::vector<std::unique_ptr<chessPiece>>>& board);
        char chessChar = ' '; //Chess piece
        bool bgColour; //Chessboard Cellcolour
        int pieceColour; //Player colour
        bool isEmpty;
        virtual ~chessPiece() = default;
};

class emptySquare : public chessPiece {
    public:
        emptySquare() {
            isEmpty = true;
        }
        int move(CursorLoc &newLoc, CursorLoc &oldLoc, std::vector<std::vector<std::unique_ptr<chessPiece>>>& board) override {return 0;};
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

class horsePiece : public chessPiece {
    public: 
        horsePiece() {
            chessChar = 'H';
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

class kingPiece : public chessPiece {
    public: 
        kingPiece() {
            chessChar = 'K';
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

class ChessBoard {
    private:
        int turnCount = 0;
        void FillRow(int row, bool& bgColour, int& unit_colour, std::vector<std::unique_ptr<chessPiece>>&Board);
//        int pawnMovement(CursorLoc &newLoc, CursorLoc &oldLoc);
    public:
        ChessBoard();
        int MovePiece(CursorLoc &newLoc, CursorLoc &oldLoc);
        //chessPiece board[BOARD_SIZE][BOARD_SIZE];
        std::vector<std::vector<std::unique_ptr<chessPiece>>> board;
};

#endif