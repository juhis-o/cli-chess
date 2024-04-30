#include <ncurses.h>
#include "chess.h"

#ifndef KIROUS_H
#define KIROUS_H

//#define ThreatDebug

struct CursorLoc {
    int8_t h,w;
}__attribute__((packed));

class ChessBoard;

class chessUI {
    private:
        CursorLoc currentLoc, selectedLoc;
        enum errList : uint8_t {DEFAULT,RED_ON_WHITE,RED_ON_BLACK,BLUE_ON_WHITE,BLUE_ON_BLACK,TEXT_COLOUR,TEMP1,TEMP2};
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