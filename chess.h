#ifndef CHESS_H
#define CHESS_H
#include "shared.h"
#include <stdlib.h>
#include <cstring>
#include <vector>
#include <memory>
#include <fstream>

class chessPiece {
    protected:
        std::vector<std::vector<std::unique_ptr<chessPiece>>>& board;
        void moveEmptySpace(CursorLoc &newLoc, CursorLoc &oldLoc);
        void moveOccupiedSpace(CursorLoc &newLoc, CursorLoc &oldLoc);
        bool calcPath(CursorLoc &newLoc, CursorLoc &oldLoc);
    public:
        chessPiece(std::vector<std::vector<std::unique_ptr<chessPiece>>>& boardRef) : board(boardRef) {}
        virtual int move(CursorLoc &newLoc, CursorLoc &oldLoc) = 0;
        virtual void checkSquares(int h, int w, std::vector<ThreatLoc>& loc) = 0;
        char chessChar = ' '; //Chess piece
        uint8_t pieceColour; //Player colour
        uint8_t piecePath[2]{false,false};
        bool pawnPath[2]{false,false};
        virtual ~chessPiece() = default;
};

class emptyPiece : public chessPiece {
    public:
        emptyPiece(std::vector<std::vector<std::unique_ptr<chessPiece>>>& boardRef) : chessPiece(boardRef){
            pieceColour = 0;
        };
        int move(CursorLoc &newLoc, CursorLoc &oldLoc) override {return 0;};
        void checkSquares(int h, int w, std::vector<ThreatLoc>& loc) override {};
};

class pawnPiece : public chessPiece {
    private: 
        bool firstMove = true;
    public: 
        pawnPiece(std::vector<std::vector<std::unique_ptr<chessPiece>>>& boardRef) : chessPiece(boardRef) {
            chessChar = 'S';
        };
        int move(CursorLoc &newLoc, CursorLoc &oldLoc) override;
        void checkSquares(int h, int w, std::vector<ThreatLoc>& loc) override;
};

class towerPiece : public chessPiece {
    public: 
        towerPiece(std::vector<std::vector<std::unique_ptr<chessPiece>>>& boardRef) : chessPiece(boardRef) {
            chessChar = 'T';
        };
        int move(CursorLoc &newLoc, CursorLoc &oldLoc) override;
        void checkSquares(int h, int w, std::vector<ThreatLoc>& loc) override;
};

class bishopPiece : public chessPiece {
    public: 
        bishopPiece(std::vector<std::vector<std::unique_ptr<chessPiece>>>& boardRef) : chessPiece(boardRef) {
            chessChar = 'B';
        };
        int move(CursorLoc &newLoc, CursorLoc &oldLoc) override;
        void checkSquares(int h, int w, std::vector<ThreatLoc>& loc) override;
};

class queenPiece : public chessPiece {
    public: 
        queenPiece(std::vector<std::vector<std::unique_ptr<chessPiece>>>& boardRef) : chessPiece(boardRef) {
            chessChar = 'Q';
        };
        int move(CursorLoc &newLoc, CursorLoc &oldLoc) override;
        void checkSquares(int h, int w, std::vector<ThreatLoc>& loc) override;
};

class horsePiece : public chessPiece {
    public: 
        horsePiece(std::vector<std::vector<std::unique_ptr<chessPiece>>>& boardRef) : chessPiece(boardRef) {
            chessChar = 'H';
        };
        int move(CursorLoc &newLoc, CursorLoc &oldLoc) override;
        void checkSquares(int h, int w, std::vector<ThreatLoc>& loc) override;
};

class kingPiece : public chessPiece {
    public: 
        kingPiece(std::vector<std::vector<std::unique_ptr<chessPiece>>>& boardRef) : chessPiece(boardRef) {
            chessChar = 'K';
        };
        int move(CursorLoc &newLoc, CursorLoc &oldLoc) override;
        void checkSquares(int h, int w, std::vector<ThreatLoc>& loc) override;
};

class ChessBoard {
    private:
        void FillRow(int row, uint8_t& unit_colour, std::vector<std::unique_ptr<chessPiece>>&Board);
        void updateThreatSquares(bool reset);
        bool canKingMove(CursorLoc& KingLoc);
        bool findOverlaps(CursorLoc& KingLoc, ThreatLoc &threatPiece);
        CursorLoc KingLoc[2] = {0};
        std::vector<std::vector<std::unique_ptr<chessPiece>>> board;
        std::vector<ThreatLoc> threatPath;
        enum rows{BACKROW,FRONTROW,EMPTYROW};
        std::ofstream file;
    public:
        ChessBoard();
        int getPieceColour(int h, int w){return board[h][w]->pieceColour;};
        int getPieceChar(int h, int w){return board[h][w]->chessChar;};
        int setPiece(CursorLoc loc, char piece, bool color);
        uint8_t* getSquareThreat(int h, int w){return board[h][w]->piecePath;};
        bool* getPawnPath(int h, int w){return board[h][w]->pawnPath;};
//        bool* getKingThreat(int iter1, int iter2){return board[iter1][iter2]->threatensKing;};
        enum CHECKMATE_STATE checkmate(bool playerTurn);
        CursorLoc* getKingPos(){return KingLoc;};
        int movePiece(CursorLoc &newLoc, CursorLoc &oldLoc){
            int ret = board[oldLoc.h][oldLoc.w]->move(newLoc,oldLoc);
            updateThreatSquares(true);
            return ret;
        };
        ~ChessBoard() {
            file.close();
        };
};

#endif