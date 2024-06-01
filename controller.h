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
        bool turn = false;
        CursorLoc cursorL[2];
        ChessBoard chessBoard;
	    chessUI userI;
    public:
        Controller();
    private:
        int movePiecetoLoc(SELECT_STATES& sel);
        int selectingPiece(SELECT_STATES& sel);
        int selection(SELECT_STATES& sel);
        int selectState(bool& quit);
};

#endif