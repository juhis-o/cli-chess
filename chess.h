#ifndef CHESS_H
#define CHESS_H
#include "shared.h"
#include <cmath>
#include <cstring>
#include <vector>
#include <memory>

const int8_t directions[8][2] = {{1,1},{-1,-1},{-1,1},{1,-1},{0,1},{1,0},{-1,0},{0,-1}};

class chessPiece {
    protected:
        std::vector<std::vector<std::unique_ptr<chessPiece>>>& board;
        void moveEmptySpace(CursorLoc &newLoc, CursorLoc &oldLoc);
        void moveOccupiedSpace(CursorLoc &newLoc, CursorLoc &oldLoc);
        bool calcPath(CursorLoc &newLoc, CursorLoc &oldLoc);
    public:
        chessPiece(std::vector<std::vector<std::unique_ptr<chessPiece>>>& boardRef, uint8_t Colour) : board(boardRef), pieceColour(Colour) {}
        virtual chessPiece_retVals move(CursorLoc &newLoc, CursorLoc &oldLoc) = 0;
        virtual void checkSquares(int8_t h, int8_t w, std::vector<ThreatLoc>& loc) = 0;
        char chessChar = ' '; //Chess piece
        uint8_t pieceColour; //Player colour
        uint8_t piecePath[2]{false,false};
        bool pawnPath[2]{false,false};
        virtual ~chessPiece() = default;
};

class emptyPiece : public chessPiece {
    public:
        emptyPiece(std::vector<std::vector<std::unique_ptr<chessPiece>>>& boardRef, uint8_t Colour) : chessPiece(boardRef,Colour){
            //pieceColour = 0;
        };
        chessPiece_retVals move(CursorLoc &newLoc, CursorLoc &oldLoc) override {return MOVE_OK;};
        void checkSquares(int8_t h, int8_t w, std::vector<ThreatLoc>& loc) override {};
};

class pawnPiece : public chessPiece {
    private: 
        bool firstMove = true;
        bool colour;
        uint8_t promoteRank;
        int8_t dirH;
    public: 
        pawnPiece(std::vector<std::vector<std::unique_ptr<chessPiece>>>& boardRef, uint8_t Colour) : chessPiece(boardRef,Colour) {
            colour = pieceColour != BLUE;
            promoteRank = (colour) ? 7 : 0;
	        dirH = (colour) ? 1 : -1;	
            chessChar = 'S';
        };
        chessPiece_retVals move(CursorLoc &newLoc, CursorLoc &oldLoc) override;
        void checkSquares(int8_t h, int8_t w, std::vector<ThreatLoc>& loc) override;
};

class towerPiece : public chessPiece {
    private:
        bool colour;
    public: 
        towerPiece(std::vector<std::vector<std::unique_ptr<chessPiece>>>& boardRef, uint8_t Colour) : chessPiece(boardRef,Colour) {
            colour = pieceColour != BLUE;
            chessChar = 'T';
        };
        chessPiece_retVals move(CursorLoc &newLoc, CursorLoc &oldLoc) override;
        void checkSquares(int8_t h, int8_t w, std::vector<ThreatLoc>& loc) override;
};

class bishopPiece : public chessPiece {
    private:
        bool colour;
    public: 
        bishopPiece(std::vector<std::vector<std::unique_ptr<chessPiece>>>& boardRef, uint8_t Colour) : chessPiece(boardRef,Colour) {
            colour = pieceColour != BLUE;
            chessChar = 'B';
        };
        chessPiece_retVals move(CursorLoc &newLoc, CursorLoc &oldLoc) override;
        void checkSquares(int8_t h, int8_t w, std::vector<ThreatLoc>& loc) override;
};

class queenPiece : public chessPiece {
    private:
        bool colour;
    public: 
        queenPiece(std::vector<std::vector<std::unique_ptr<chessPiece>>>& boardRef, uint8_t Colour) : chessPiece(boardRef,Colour) {
            colour = pieceColour != BLUE;
            chessChar = 'Q';
        };
        chessPiece_retVals move(CursorLoc &newLoc, CursorLoc &oldLoc) override;
        void checkSquares(int8_t h, int8_t w, std::vector<ThreatLoc>& loc) override;
};

class horsePiece : public chessPiece {
    private:
        bool colour;
    public: 
        horsePiece(std::vector<std::vector<std::unique_ptr<chessPiece>>>& boardRef, uint8_t Colour) : chessPiece(boardRef,Colour) {
            colour = pieceColour != BLUE;
            chessChar = 'H';
        };
        chessPiece_retVals move(CursorLoc &newLoc, CursorLoc &oldLoc) override;
        void checkSquares(int8_t h, int8_t w, std::vector<ThreatLoc>& loc) override;
};

class kingPiece : public chessPiece {
    private:
        bool colour;
    public: 
        kingPiece(std::vector<std::vector<std::unique_ptr<chessPiece>>>& boardRef, uint8_t Colour) : chessPiece(boardRef,Colour) {
            colour = pieceColour != BLUE;
            chessChar = 'K';
        };
        chessPiece_retVals move(CursorLoc &newLoc, CursorLoc &oldLoc) override;
        void checkSquares(int8_t h, int8_t w, std::vector<ThreatLoc>& loc) override;
};

class ChessBoard {
    private:
        void FillRow(uint8_t row, uint8_t& unit_colour, std::vector<std::unique_ptr<chessPiece>>&Board);
        void updateThreatSquares(bool reset);
        bool canKingMove(CursorLoc& KingLoc);
        bool findOverlaps(CursorLoc& KingLoc, ThreatLoc &threatPiece);
        CursorLoc KingLoc[2] = {0};
        std::vector<std::vector<std::unique_ptr<chessPiece>>> board;
        std::vector<ThreatLoc> threatPath;
        enum rows{BACKROW,FRONTROW,EMPTYROW};
    public:
        ChessBoard();
        uint8_t getPieceColour(int8_t h, int8_t w){return board[h][w]->pieceColour;};
        uint8_t* getSquareThreat(int8_t h,int8_t w){return board[h][w]->piecePath;};
        char getPieceChar(int8_t h, int8_t w){return board[h][w]->chessChar;};
        chessPiece_retVals setPiece(CursorLoc loc, char piece, bool color);
        bool* getPawnPath(int8_t h, int8_t w){return board[h][w]->pawnPath;};
        enum CHECKMATE_STATE checkmate(bool playerTurn);
        CursorLoc* getKingPos(){return KingLoc;};
        chessPiece_retVals movePiece(CursorLoc &newLoc, CursorLoc &oldLoc){
            chessPiece_retVals ret = board[oldLoc.h][oldLoc.w]->move(newLoc,oldLoc);
            updateThreatSquares(true);
            return ret;
        };
        ~ChessBoard() {};
};

#endif