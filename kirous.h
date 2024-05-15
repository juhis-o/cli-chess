#include <ncurses.h>
#include "chess.h"

#ifndef KIROUS_H
#define KIROUS_H

#define ThreatDebug

struct CursorLoc {
    int8_t h = 0, w = 0;
}__attribute__((packed));

class ChessBoard;

class chessUI {
    private:
        CursorLoc currentLoc, selectedLoc;
        enum errList : uint8_t {DEFAULT,RED_ON_WHITE,RED_ON_BLACK,BLUE_ON_WHITE,BLUE_ON_BLACK,TEXT_COLOUR,TEMP1,TEMP2,TEMP3,TEMP4};
        bool Init = false;
        int ch;
    public:
        chessUI();
        ~chessUI();
        int Select(CursorLoc& loc);
        void updateInterface(ChessBoard &cBoard, int ret,char* debug);

};

#endif