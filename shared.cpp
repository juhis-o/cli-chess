#include "shared.h"
bool inBounds(int8_t h, int8_t w){
    return (h >= 0) && (h < BOARD_SIZE) && (w >= 0) && (w < BOARD_SIZE);
    }