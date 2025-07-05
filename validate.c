#include "defs.h"

int SqOnBoard(const int sq) { 
    return FilesBoard[sq] == OFF_BOARD ? 0 : 1;
}

int SideValid(const int side) { 
    return (side==WHITE || side == BLACK ) ? 1 : 0;
}

int FileRankValid(const int fr) {
    return (fr >= 0 && fr <= 7) ? 1: 0;
}

int PieceValidEmpty(const int pce) { 
    return (pce >= EMPTY && pce <= bK) ? 1 : 0;
}

int PieceValid(const int pce) { 
    return (pce >= wP && pce <=bK) ? 1 : 0;
}

int SqIs120(const int sq) {
	return (sq>=0 && sq<120);
}

int PceValidEmptyOffbrd(const int pce) {
	return (PieceValidEmpty(pce) || pce == OFF_BOARD);
}