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
        std::vector<std::vector<std::unique_ptr<chessPiece>>>& board;
        int moveEmptySpace(CursorLoc &newLoc, CursorLoc &oldLoc);
        int moveOccupiedSpace(CursorLoc &newLoc, CursorLoc &oldLoc);
        bool calcPath(CursorLoc &newLoc, CursorLoc &oldLoc);
    public:
        chessPiece(std::vector<std::vector<std::unique_ptr<chessPiece>>>& boardRef) : board(boardRef) {}
        virtual int move(CursorLoc &newLoc, CursorLoc &oldLoc) = 0;
        char chessChar = ' '; //Chess piece
        int pieceColour; //Player colour
        virtual ~chessPiece() = default;
};

class pawnPiece : public chessPiece {
    public: 
        pawnPiece(std::vector<std::vector<std::unique_ptr<chessPiece>>>& boardRef) : chessPiece(boardRef) {
            chessChar = 'S';
        };
        int move(CursorLoc &newLoc, CursorLoc &oldLoc) override;
        bool firstMove = true;
};

class towerPiece : public chessPiece {
    public: 
        towerPiece(std::vector<std::vector<std::unique_ptr<chessPiece>>>& boardRef) : chessPiece(boardRef) {
            chessChar = 'T';
        };
        int move(CursorLoc &newLoc, CursorLoc &oldLoc) override;
};

class bishopPiece : public chessPiece {
    public: 
        bishopPiece(std::vector<std::vector<std::unique_ptr<chessPiece>>>& boardRef) : chessPiece(boardRef) {
            chessChar = 'B';
        };
        int move(CursorLoc &newLoc, CursorLoc &oldLoc) override;
};

class queenPiece : public chessPiece {
    public: 
        queenPiece(std::vector<std::vector<std::unique_ptr<chessPiece>>>& boardRef) : chessPiece(boardRef) {
            chessChar = 'Q';
        };
        int move(CursorLoc &newLoc, CursorLoc &oldLoc) override;
};

class horsePiece : public chessPiece {
    public: 
        horsePiece(std::vector<std::vector<std::unique_ptr<chessPiece>>>& boardRef) : chessPiece(boardRef) {
            chessChar = 'H';
        };
        int move(CursorLoc &newLoc, CursorLoc &oldLoc) override;
};

class kingPiece : public chessPiece {
    public: 
        kingPiece(std::vector<std::vector<std::unique_ptr<chessPiece>>>& board) : chessPiece(board) {
            chessChar = 'K';
        };
        int move(CursorLoc &newLoc, CursorLoc &oldLoc) override;
};

class ChessBoard {
    private:
        int turnCount = 0;
        void FillRow(int row, int& unit_colour, std::vector<std::unique_ptr<chessPiece>>&Board);
        std::vector<std::vector<std::unique_ptr<chessPiece>>> board;
    public:
        ChessBoard();
        int getPieceColour(int iter1, int iter2){return board[iter1][iter2]->pieceColour;};
        int getPieceChar(int iter1, int iter2){return board[iter1][iter2]->chessChar;};
        bool isNull(int iter1, int iter2){return (board[iter1][iter2] == nullptr) ? true : false;};
        int movePiece(int iter1, int iter2,CursorLoc &newLoc, CursorLoc &oldLoc){return board[iter1][iter2]->move(newLoc,oldLoc);};
};

#endif