#include <iostream>
#include "chess.h"
#include "kirous.h"

int main(){
	int retVal = 0;
	bool playerTurn = 1;
	CursorLoc cursorL[2];
	CursorLoc* kings;
	ChessBoard chessBoard;
	chessUI userI;

	char debug[100];

	char c = 0;
	int color = 0;

	while(retVal != -6) {
		for(int SelectState = 0; SelectState < 2;){
			userI.updateInterface(chessBoard, retVal, debug);
			if((retVal = userI.Select(cursorL[SelectState])) == -6) break; 
			if (cursorL[SelectState].h >= 0 && cursorL[SelectState].h < BOARD_SIZE && cursorL[SelectState].w >= 0 && cursorL[SelectState].w < BOARD_SIZE) {
				if(!SelectState) {
					c = chessBoard.getPieceChar(cursorL[0].h,cursorL[0].w);
					color = chessBoard.getPieceColour(cursorL[0].h,cursorL[0].w);
					if((c != ' ') && ((playerTurn && color == BLUE) || 
			 		(!playerTurn && color == RED))) {
						SelectState++;
					}
				}
				else {
					if((retVal = chessBoard.movePiece(cursorL[1],cursorL[0], playerTurn)) == 0) {
						playerTurn = !playerTurn;
						SelectState++;
					}
					else SelectState--;
				}
			}
			else retVal = -100;
		}
		kings = chessBoard.getKingPos();
		snprintf(debug,100,"%d, %d, %d, %d", kings[0].h,kings[0].w,kings[1].h,kings[1].w);
	}
	return 0;
}

