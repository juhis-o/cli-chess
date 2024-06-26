#ifdef _WIN32
#include <curses.h>
#else
#include <ncurses.h>
#endif
#include "shared.h"
#include "chess.h"

#ifndef KIROUS_H
#define KIROUS_H

class ChessBoard;

class chessUI {
    private:
        CursorLoc currentLoc, selectedLoc;
        enum colourList : uint8_t {DEFAULT,RED_ON_WHITE,RED_ON_BLACK,BLUE_ON_WHITE,BLUE_ON_BLACK,TEXT_COLOUR,TEMP1,TEMP2,TEMP3,TEMP4};
        bool Init = false;
        int8_t gameover = false;
        int ch;
    public:
        chessUI();
        ~chessUI();
        bool Select(CursorLoc& loc);
        char promotionSelect();
        void updateInterface(ChessBoard &cBoard, chessPiece_retVals ret, enum CHECKMATE_STATE check);
        void printSelectState(ChessBoard &cBoard, chessPiece_retVals msg);
};

#endif