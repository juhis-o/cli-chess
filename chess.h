#ifndef CHESS_H
#define CHESS_H
#include "kirous.h"
#include <stdlib.h>
#include <vector>
#include <memory>

#define BOARD_SIZE 8
#define RED 1
#define BLUE 3

class chessPiece {
    protected:
        std::vector<std::vector<std::unique_ptr<chessPiece>>>& board;
        void moveEmptySpace(CursorLoc &newLoc, CursorLoc &oldLoc);
        void moveOccupiedSpace(CursorLoc &newLoc, CursorLoc &oldLoc);
        bool calcPath(CursorLoc &newLoc, CursorLoc &oldLoc);
        enum errList{MOVE_OK,MOVE_NOT_VALID,CAPTURING_OWN_PIECE,PIECE_ON_PATH,MOVE_CANCEL};
    public:
        chessPiece(std::vector<std::vector<std::unique_ptr<chessPiece>>>& boardRef) : board(boardRef) {}
        virtual int move(CursorLoc &newLoc, CursorLoc &oldLoc) = 0;
        virtual void checkSquares(int h, int w) = 0;
        char chessChar = ' '; //Chess piece
        int pieceColour; //Player colour
        bool threat[2]{false,false};
        virtual ~chessPiece() = default;
};

class emptyPiece : public chessPiece {
    public: 
        emptyPiece(std::vector<std::vector<std::unique_ptr<chessPiece>>>& boardRef) : chessPiece(boardRef){};
        int move(CursorLoc &newLoc, CursorLoc &oldLoc) override {return 0;};
        void checkSquares(int h, int w) override {};
};

class pawnPiece : public chessPiece {
    private: 
        bool firstMove = true;
    public: 
        pawnPiece(std::vector<std::vector<std::unique_ptr<chessPiece>>>& boardRef) : chessPiece(boardRef) {
            chessChar = 'S';
        };
        int move(CursorLoc &newLoc, CursorLoc &oldLoc) override;
        void checkSquares(int h, int w) override;
};

class towerPiece : public chessPiece {
    public: 
        towerPiece(std::vector<std::vector<std::unique_ptr<chessPiece>>>& boardRef) : chessPiece(boardRef) {
            chessChar = 'T';
        };
        int move(CursorLoc &newLoc, CursorLoc &oldLoc) override;
        void checkSquares(int h, int w) override;
};

class bishopPiece : public chessPiece {
    public: 
        bishopPiece(std::vector<std::vector<std::unique_ptr<chessPiece>>>& boardRef) : chessPiece(boardRef) {
            chessChar = 'B';
        };
        int move(CursorLoc &newLoc, CursorLoc &oldLoc) override;
        void checkSquares(int h, int w) override;
};

class queenPiece : public chessPiece {
    public: 
        queenPiece(std::vector<std::vector<std::unique_ptr<chessPiece>>>& boardRef) : chessPiece(boardRef) {
            chessChar = 'Q';
        };
        int move(CursorLoc &newLoc, CursorLoc &oldLoc) override;
        void checkSquares(int h, int w) override;
};

class horsePiece : public chessPiece {
    public: 
        horsePiece(std::vector<std::vector<std::unique_ptr<chessPiece>>>& boardRef) : chessPiece(boardRef) {
            chessChar = 'H';
        };
        int move(CursorLoc &newLoc, CursorLoc &oldLoc) override;
        void checkSquares(int h, int w) override;
};

class kingPiece : public chessPiece {
    public: 
        kingPiece(std::vector<std::vector<std::unique_ptr<chessPiece>>>& boardRef) : chessPiece(boardRef) {
            chessChar = 'K';
        };
        int move(CursorLoc &newLoc, CursorLoc &oldLoc) override;
        void checkSquares(int h, int w) override;
};

class ChessBoard {
    private:
        int turnCount = 0;
        void FillRow(int row, int& unit_colour, std::vector<std::unique_ptr<chessPiece>>&Board);
        std::vector<std::vector<std::unique_ptr<chessPiece>>> board;
        enum rows{BACKROW,FRONTROW,EMPTYROW};
    public:
        ChessBoard();
        int getPieceColour(int iter1, int iter2){return board[iter1][iter2]->pieceColour;};
        int getPieceChar(int iter1, int iter2){return board[iter1][iter2]->chessChar;};
        bool getSquareThreat(int iter1, int iter2){return board[iter1][iter2]->threat[1];};
        int movePiece(CursorLoc &newLoc, CursorLoc &oldLoc){
            int a = board[oldLoc.h][oldLoc.w]->move(newLoc,oldLoc);
            for (int i = 0; i < BOARD_SIZE; i++){
		        for(int j = 0; j < BOARD_SIZE; j++){
			        board[i][j]->threat[0] = false;
                    board[i][j]->threat[1] = false;
		        }
	        }
            for (int i = 0; i < BOARD_SIZE; i++){
		        for(int j = 0; j < BOARD_SIZE; j++){
			        if(board[i][j]->chessChar != ' ') board[i][j]->checkSquares(i,j);
		        }
	        }
            return a;};
};

#endif