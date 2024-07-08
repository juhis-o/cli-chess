#ifndef SHARED_H
#define SHARED_H
#include <cstdint>

#define RED 1
#define BLUE 3
#define BOARD_SIZE 8

enum chessPiece_retVals : int8_t {INVALID_SELECT = -11, THREAT, CANT_RISK_OWN_KING, CASTLING_PATH_FAIL, TOWER_HAS_MOVED, PAWN_WRONG_DIR, PAWN_NO_PIECE_TO_CAPTURE, MOVE_NOT_VALID, CAPTURING_OWN_PIECE, PIECE_ON_PATH,MOVE_CANCEL, SELECT_OK, MOVE_OK, PAWN_PROMOTION, PAWN_PROMOTED};
enum CHECKMATE_STATE{KING_THREATENED = -1, KING_SAFE, KING_PROTECT_FAIL, CHECKMATE};

#ifdef _WIN32
#pragma pack(push,1)
struct CursorLoc {
    int8_t h = 0, w = 0;
};

struct ThreatLoc {
    int8_t h = 0, w = 0;
    bool horsePiece = false;
};
#pragma pack(pop)
#else
struct CursorLoc {
    int8_t h = 0, w = 0;
}__attribute__((packed));

struct ThreatLoc {
    int8_t h = 0, w = 0;
    bool horsePiece = false;
}__attribute__((packed));
#endif

bool inBounds(int8_t h, int8_t w);
#endif