#include "shared.h"
bool iswithinBoard(CursorLoc& loc){
    return loc.h >= 0 && loc.h < BOARD_SIZE &&
           loc.w >= 0 && loc.w < BOARD_SIZE;
    }

bool inBounds(int8_t h, int8_t w){
    return (h >= 0) && (h < BOARD_SIZE) && (w >= 0) && (w < BOARD_SIZE);
    }