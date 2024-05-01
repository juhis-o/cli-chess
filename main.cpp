#include <iostream>
#include "chess.h"
#include "kirous.h"

int main(){
	int retVal = 0;
	CursorLoc cursorL[2];
	bool playerTurn = 1;
	ChessBoard chessBoard;
	chessUI userI;

	char c = 0;
	int color = 0;

	while(retVal -= -6) {
		for(int i = 0; i < 2;){
			userI.updateInterface(chessBoard, retVal);
			if((retVal = userI.Select(cursorL[i])) == -6) break;
			if (cursorL[i].h >= 0 && cursorL[i].h < BOARD_SIZE &&
            cursorL[i].w >= 0 && cursorL[i].w < BOARD_SIZE) {
				if(!i) {
					c = chessBoard.getPieceChar(cursorL[0].h,cursorL[0].w);
					color = chessBoard.getPieceColour(cursorL[0].h,cursorL[0].w);
					if((c != ' ') && ((playerTurn && color == BLUE) || 
			 		(!playerTurn && color == RED))) {
						i++;
					}
				}
				else {
					if((retVal = chessBoard.movePiece(cursorL[1],cursorL[0])) == 0) {
						playerTurn = !playerTurn;
						i++;
					}
					else i--;
				}
			}
			else retVal = -100;
		}
	}


	return 0;
}

