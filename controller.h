#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "chess.h"
#include "kirous.h"
#include "shared.h"

class Controller {
    private:
        enum CHESS_STATES{SELECT,PROMOTE};
        enum SELECT_STATES{SELECT_PIECE, SELECT_LOCATION, END_SELECT};
        CHESS_STATES gameState = SELECT;
        CHECKMATE_STATE checkState = KING_SAFE;
        const int8_t playerColours[2] = {BLUE,RED};
        bool turn = false;
        CursorLoc cursorL[2];
        ChessBoard chessBoard;
	    chessUI userI;
    public:
        Controller();
    private:
        chessPiece_retVals movePiecetoLoc(SELECT_STATES& sel);
        chessPiece_retVals selectingPiece(SELECT_STATES& sel);
        chessPiece_retVals selection(SELECT_STATES& sel);
        chessPiece_retVals selectState(bool& quit);
};

#endif