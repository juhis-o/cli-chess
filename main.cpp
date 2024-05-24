#include <iostream>
#include "chess.h"
#include "kirous.h"
#include "shared.h"

int main(){
	int retVal = 0;
	bool playerTurn = 1;
	CursorLoc cursorL[2];
	ChessBoard chessBoard;
	chessUI userI;
	int8_t gameState = false;
	bool KingThreat = false;
	char c = 0;
	int color = 0;

	while(retVal != -6) {
		for(int SelectState = 0; SelectState < 2;){
			userI.updateInterface(chessBoard, retVal, gameState);
			if((retVal = userI.Select(cursorL[SelectState])) == -6) break; 
			if (cursorL[SelectState].h >= 0 && cursorL[SelectState].h < BOARD_SIZE && cursorL[SelectState].w >= 0 && cursorL[SelectState].w < BOARD_SIZE) {
				if(!SelectState) {
					c = chessBoard.getPieceChar(cursorL[0].h,cursorL[0].w);
					color = chessBoard.getPieceColour(cursorL[0].h,cursorL[0].w);
					if((c != ' ') && ((playerTurn && color == BLUE) || 
			 		(!playerTurn && color == RED))) 
						SelectState++;
				}
				else {
					if((retVal = chessBoard.movePiece(cursorL[1],cursorL[0])) == 0) {
						SelectState++;
						if(KingThreat) {
							if(chessBoard.checkmate(!playerTurn) == -1) gameState = -2;
							else KingThreat = false;
						}
					}

					else SelectState--;
				}
			}
			else retVal = -100;
		}
		if(gameState != -2) {
			gameState = chessBoard.checkmate(playerTurn);
			if(gameState == -1) KingThreat = true;
			playerTurn = !playerTurn;
		}
	}
	return 0;
}