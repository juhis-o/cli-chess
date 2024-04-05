#include <iostream>
#include <cstring>
#include "chess.h"
#include "kirous.h"

int main(){
	
	//bool playerTurn = 0;
	
	ChessBoard chessBoard;
	chessUI userI;
	userI.updateInterface(chessBoard);
	userI.freeSelect(chessBoard);

	return 0;
}

