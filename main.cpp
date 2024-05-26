#include <iostream>
#include "chess.h"
#include "kirous.h"
#include "shared.h"

int main(){
	int retVal = 0;
	int color = 0;
	int8_t gameState = false;
	char c = 0;
	bool playerTurn = 1;
	bool KingThreat = false;
	CursorLoc cursorL[2];
	CursorLoc selectedPiece;
	ChessBoard chessBoard;
	chessUI userI;

	while(retVal != -6) {
		for(int SelectState = 0; SelectState < 2;){
			userI.updateInterface(chessBoard, retVal, gameState, SelectState);
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