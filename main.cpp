#include <iostream>
#include "chess.h"
#include "kirous.h"
#include "shared.h"

enum CHESS_STATES{SELECT,PROMOTE};
enum SELECT_STATES{SELECT_PIECE, SELECT_LOCATION, END_SELECT};

int main(){
	ChessBoard chessBoard;
	chessUI userI;
	CHECKMATE_STATE checkState = KING_SAFE;
	CHESS_STATES gameState = SELECT;
	int retVal = 0;
	CursorLoc cursorL[2];

	int8_t players[2] = {RED,BLUE};
	bool turn = true;
	bool quit = false;

	while(!quit) {
		userI.updateInterface(chessBoard, retVal, checkState);
		switch(gameState) {
			case SELECT:
				for(SELECT_STATES selectState = SELECT_PIECE; selectState < END_SELECT;){
					if((quit = userI.Select(cursorL[selectState]))) break;
					if(iswithinBoard(cursorL[selectState])){
						switch(selectState) {
							case SELECT_PIECE:
								if(chessBoard.getPieceColour(cursorL[selectState].h,cursorL[selectState].w) == players[turn]) {
									selectState = SELECT_LOCATION;
									retVal = SELECT_OK;
								} 
								else retVal = INVALID_SELECT; //Invalid selection
								break;
							case SELECT_LOCATION:
								if((retVal = chessBoard.movePiece(cursorL[SELECT_LOCATION],cursorL[SELECT_PIECE])) > -1) {
									if(retVal == PAWN_PROMOTION) gameState = PROMOTE;
									selectState = END_SELECT;
								}
								else selectState = SELECT_PIECE;
								break;
							case END_SELECT:
								break;
						}
					}
					else retVal = INVALID_SELECT; //Out of bounds
					if(retVal < 1) userI.printSelectState(chessBoard,retVal);
				}
				checkState = chessBoard.checkmate(turn);
				turn = !turn;
				break;
			case PROMOTE:
				char c = userI.promotionSelect();
				retVal = chessBoard.setPiece(cursorL[SELECT_LOCATION],c,!turn);
				gameState = SELECT;
				checkState = chessBoard.checkmate(!turn);
				break;
		}
	}
	return 0;
}