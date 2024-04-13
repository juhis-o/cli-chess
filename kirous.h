#include <ncurses.h>
#include "chess.h"

#ifndef KIROUS_H
#define KIROUS_H

#define TEXT_COLOUR 5

struct CursorLoc {
    int h,w;
};

class ChessBoard;

class chessUI {
    private:
        CursorLoc currentLoc, selectedLoc;
        bool selected = false;
        bool Init = false;
        bool playerTurn = true;
        int ch;
        int ret = 0;

    public:
        chessUI();
        ~chessUI();
        void freeSelect(ChessBoard &cBoard);
        void enterPressed(ChessBoard &cBoard);
        void updateInterface(ChessBoard &cBoard);

};

#endif