#include <iostream>
#include "chess.h"
#include "kirous.h"
#include "shared.h"

enum CHESS_STATES{SELECT,PROMOTE};
enum SELECT_STATES{SELECT_PIECE, SELECT_LOCATION, END_SELECT};
CHESS_STATES gameState = SELECT;
bool turn = true;
int8_t players[2] = {RED,BLUE};

inline static int movePiecetoLoc(SELECT_STATES& sel, ChessBoard& chessBoard, CursorLoc* cursorL){
	int ret = 0;
	if((ret = chessBoard.movePiece(cursorL[SELECT_LOCATION],cursorL[SELECT_PIECE])) > -1) {
		if(ret == PAWN_PROMOTION) gameState = PROMOTE;
		sel = END_SELECT;
	}
	else sel = SELECT_PIECE;

	return ret;
}

inline static int selectingPiece(SELECT_STATES& sel, ChessBoard& chessBoard, CursorLoc& selectedPiece){
	int ret = 0;
	if(chessBoard.getPieceColour(selectedPiece.h,selectedPiece.w) == players[turn]) {
		sel = SELECT_LOCATION;
		ret = SELECT_OK;
	} 
	else ret = INVALID_SELECT; //Invalid selection

	return ret;
}

inline static int selection(SELECT_STATES& sel, ChessBoard& chessBoard, CursorLoc* cursorL) {
	int retVal = 0;
	switch(sel) {
		case SELECT_PIECE:
			retVal = selectingPiece(sel,chessBoard,cursorL[SELECT_PIECE]);
			break;
		case SELECT_LOCATION:
			retVal = movePiecetoLoc(sel,chessBoard,cursorL);
			break;
		case END_SELECT:
			break;
	}
	return retVal;
}

inline static int selectState(bool& quit, CursorLoc& select_location, ChessBoard& chessBoard, chessUI& userI) {
	int retVal = 0;
	CursorLoc cursorL[2];
	for(SELECT_STATES selectState = SELECT_PIECE; selectState < END_SELECT;){
		if((quit = userI.Select(cursorL[selectState]))) break;
		if(iswithinBoard(cursorL[selectState])){
		retVal = selection(selectState, chessBoard, cursorL);
		}
		else retVal = INVALID_SELECT; //Out of bounds
		if(retVal < 1) userI.printSelectState(chessBoard,retVal);
	}
	select_location = cursorL[SELECT_LOCATION];
	return retVal;
}


int main(){
	ChessBoard chessBoard;
	chessUI userI;
	CHECKMATE_STATE checkState = KING_SAFE;
	int retVal = 0;
	bool quit = false;
	CursorLoc pawnPromote;

	while(!quit) {
		userI.updateInterface(chessBoard, retVal, checkState);
		switch(gameState) {
			case SELECT:
				retVal = selectState(quit, pawnPromote, chessBoard, userI);
				checkState = chessBoard.checkmate(turn);
				turn = !turn;
				break;
			case PROMOTE:
				char c = userI.promotionSelect();
				retVal = chessBoard.setPiece(pawnPromote,c,!turn);
				gameState = SELECT;
				checkState = chessBoard.checkmate(!turn);
				break;
		}
	}
	return 0;
}