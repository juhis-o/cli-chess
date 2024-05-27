#ifndef SHARED_H
#define SHARED_H
#include <cstdint>

#define RED 1
#define BLUE 3
#define BOARD_SIZE 8

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
#endif