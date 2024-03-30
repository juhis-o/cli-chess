#include <iostream>
#include <cstring>
#include "chess.h"
#include "kirous.h"

int main(){
	ChessBoard chessBoard;
	chessUI userI;
	userI.updateInterface(chessBoard);
	userI.freeSelect(chessBoard);
	
	return 0;
}
