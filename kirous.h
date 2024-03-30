#include <ncurses.h>
#include "chess.h"

#ifndef KIROUS_H
#define KIROUS_H

struct CursorLoc {
    int h,w;
};

class ChessBoard;

class chessUI {
    private:
        CursorLoc currentLoc, selectedLoc;
        bool selected;
        bool Init = false;
        int ch;

    public:
        chessUI();
        ~chessUI();
        void freeSelect(ChessBoard &cBoard);
        void enterPressed(ChessBoard &cBoard);
        void updateInterface(ChessBoard &cBoard);

};

#endif