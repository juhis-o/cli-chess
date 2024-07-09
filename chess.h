#ifndef CHESS_H
#define CHESS_H
#include "shared.h"
#include <cmath>
#include <cstring>
#include <vector>
#include <memory>

const int8_t directions[8][2] = {{1,1},{-1,-1},{-1,1},{1,-1},{0,1},{1,0},{-1,0},{0,-1}}; //For king, queen, bishop and tower(rook)

class chessPiece {
    protected:
        std::vector<std::vector<std::unique_ptr<chessPiece>>>& board;
        void moveEmptySpace(CursorLoc &newLoc, CursorLoc &oldLoc);
        void moveOccupiedSpace(CursorLoc &newLoc, CursorLoc &oldLoc);
        bool calcPath(CursorLoc &newLoc, CursorLoc &oldLoc);
    public:
        chessPiece(std::vector<std::vector<std::unique_ptr<chessPiece>>>& boardRef, uint8_t Colour) : board(boardRef), pieceColour(Colour){}
        virtual std::unique_ptr<chessPiece> clone() const = 0;
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
        emptyPiece(std::vector<std::vector<std::unique_ptr<chessPiece>>>& boardRef, uint8_t Colour) : chessPiece(boardRef,Colour){};
        chessPiece_retVals move(CursorLoc &newLoc, CursorLoc &oldLoc) override {(void)newLoc;(void)oldLoc; return MOVE_CANCEL;};
        void checkSquares(int8_t h, int8_t w, std::vector<ThreatLoc>& loc) override {(void)h;(void)w,(void)loc;};
        std::unique_ptr<chessPiece> clone() const override {return std::make_unique<emptyPiece>(*this);};
};

class pawnPiece : public chessPiece {
    private: 
        bool firstMove = true;
        bool colour;
        uint8_t promoteRank;
        uint8_t enPassantRank;
        int8_t dirH;
        chessPiece_retVals forwardMove(CursorLoc &newLoc, CursorLoc &oldLoc, int8_t &movementY);
        chessPiece_retVals captureMove(CursorLoc &newLoc, CursorLoc &oldLoc);
        void enPassantCapture(CursorLoc &newLoc, CursorLoc &oldLoc, int8_t h);
    public: 
        pawnPiece(std::vector<std::vector<std::unique_ptr<chessPiece>>>& boardRef, uint8_t Colour) : chessPiece(boardRef,Colour){
            colour = pieceColour != BLUE;
            promoteRank = (colour) ? 7 : 0;
            enPassantRank = (colour) ? 4 : 3;
            dirH = (colour) ? 1 : -1;
            chessChar = 'S';
        };
        chessPiece_retVals move(CursorLoc &newLoc, CursorLoc &oldLoc) override;
        void checkSquares(int8_t h, int8_t w, std::vector<ThreatLoc>& loc) override;
        std::unique_ptr<chessPiece> clone() const override {
        return std::make_unique<pawnPiece>(*this);
    }
        bool enPassantable = false;
};

class towerPiece : public chessPiece {
    private:
        bool colour;
    public: 
        towerPiece(std::vector<std::vector<std::unique_ptr<chessPiece>>>& boardRef, uint8_t Colour) : chessPiece(boardRef,Colour){
            colour = pieceColour != BLUE;
            chessChar = 'T';
        };
        bool firstMove = true;
        chessPiece_retVals move(CursorLoc &newLoc, CursorLoc &oldLoc) override;
        void checkSquares(int8_t h, int8_t w, std::vector<ThreatLoc>& loc) override;
        std::unique_ptr<chessPiece> clone() const override {
        return std::make_unique<towerPiece>(*this);
    }
};

class bishopPiece : public chessPiece {
    private:
        bool colour;
    public: 
        bishopPiece(std::vector<std::vector<std::unique_ptr<chessPiece>>>& boardRef, uint8_t Colour) : chessPiece(boardRef,Colour){
            colour = pieceColour != BLUE;
            chessChar = 'B';
        };
        chessPiece_retVals move(CursorLoc &newLoc, CursorLoc &oldLoc) override;
        void checkSquares(int8_t h, int8_t w, std::vector<ThreatLoc>& loc) override;
        std::unique_ptr<chessPiece> clone() const override {
        return std::make_unique<bishopPiece>(*this);
    }
};

class queenPiece : public chessPiece {
    private:
        bool colour;
    public: 
        queenPiece(std::vector<std::vector<std::unique_ptr<chessPiece>>>& boardRef, uint8_t Colour) : chessPiece(boardRef,Colour){
            colour = pieceColour != BLUE;
            chessChar = 'Q';
        };
        chessPiece_retVals move(CursorLoc &newLoc, CursorLoc &oldLoc) override;
        void checkSquares(int8_t h, int8_t w, std::vector<ThreatLoc>& loc) override;
        std::unique_ptr<chessPiece> clone() const override {
        return std::make_unique<queenPiece>(*this);
    }
};

class horsePiece : public chessPiece {
    private:
        bool colour;
    public: 
        horsePiece(std::vector<std::vector<std::unique_ptr<chessPiece>>>& boardRef, uint8_t Colour) : chessPiece(boardRef,Colour){
            colour = pieceColour != BLUE;
            chessChar = 'H';
        };
        chessPiece_retVals move(CursorLoc &newLoc, CursorLoc &oldLoc) override;
        void checkSquares(int8_t h, int8_t w, std::vector<ThreatLoc>& loc) override;
        std::unique_ptr<chessPiece> clone() const override {
        return std::make_unique<horsePiece>(*this);
    }       
};

class kingPiece : public chessPiece {
    private:
        bool firstMove = true;
        bool colour;
    public: 
        kingPiece(std::vector<std::vector<std::unique_ptr<chessPiece>>>& boardRef, uint8_t Colour) : chessPiece(boardRef,Colour){
            colour = pieceColour != BLUE;
            chessChar = 'K';
        };
        chessPiece_retVals castling(CursorLoc& newLoc, CursorLoc& oldLoc);
        bool pathClear(CursorLoc &newLoc, CursorLoc &oldLoc);
        void moveCastling(CursorLoc &tower, CursorLoc &king);
        chessPiece_retVals move(CursorLoc &newLoc, CursorLoc &oldLoc) override;
        void checkSquares(int8_t h, int8_t w, std::vector<ThreatLoc>& loc) override;
        std::unique_ptr<chessPiece> clone() const override {
        return std::make_unique<kingPiece>(*this);
    }

};

class ChessBoard {
    private:
        void FillRow(uint8_t row, uint8_t& unit_colour, std::vector<std::unique_ptr<chessPiece>>&Board);
        void updateThreatSquares(bool reset);
        void updateThreatSquares(bool reset, bool turn);
        bool canKingMove(CursorLoc& king);
        bool findOverlaps(CursorLoc& KingLoc, ThreatLoc &threatPiece);
        CursorLoc KingLoc[2] = {0};
        std::vector<std::vector<std::unique_ptr<chessPiece>>> board;
        std::vector<ThreatLoc> threatPath;
        enum rows{BACKROW,FRONTROW,EMPTYROW};
    public:
        ChessBoard();
        const int8_t playerColours[2] = {BLUE,RED};
        uint8_t getPieceColour(int8_t h, int8_t w){return board[h][w]->pieceColour;};
        uint8_t* getSquareThreat(int8_t h,int8_t w){return board[h][w]->piecePath;};
        char getPieceChar(int8_t h, int8_t w){return board[h][w]->chessChar;};
        chessPiece_retVals setPiece(CursorLoc loc, char piece, bool color);
        bool* getPawnPath(int8_t h, int8_t w){return board[h][w]->pawnPath;};
        enum CHECKMATE_STATE checkmate(bool playerTurn);
        CursorLoc* getKingPos(){return KingLoc;};
        chessPiece_retVals movePiece(CursorLoc &newLoc, CursorLoc &oldLoc, bool playerTurn);
        std::vector<std::vector<std::unique_ptr<chessPiece>>> deepCopyBoard(const std::vector<std::vector<std::unique_ptr<chessPiece>>>& board);
        ~ChessBoard(){};
};

#endif