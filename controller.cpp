#include "controller.h"

Controller::Controller(){
    bool quit = false;
    int retVal = 0;
    while(!quit) {
    userI.updateInterface(chessBoard, retVal, checkState);
    switch(gameState) {
        case SELECT:
            retVal = selectState(quit);
            turn = !turn;
			checkState = chessBoard.checkmate(turn);
            break;
        case PROMOTE:
            char c = userI.promotionSelect();
            retVal = chessBoard.setPiece(cursorL[SELECT_LOCATION],c,turn);
            gameState = SELECT;
            checkState = chessBoard.checkmate(turn);
            break;
        }
    }
}

int Controller::selectState(bool& quit){
	int retVal = 0;
	for(SELECT_STATES selectState = SELECT_PIECE; selectState < END_SELECT;){
		if((quit = userI.Select(cursorL[selectState]))) break;
		if(iswithinBoard(cursorL[selectState])){
			retVal = selection(selectState);
		}
		else retVal = INVALID_SELECT; //Out of bounds
		if(retVal < 1) userI.printSelectState(chessBoard,retVal);
	}
	return retVal;
}

int Controller::selection(SELECT_STATES& sel){
	int retVal = 0;
	switch(sel) {
		case SELECT_PIECE:
			retVal = selectingPiece(sel);
			break;
		case SELECT_LOCATION:
			retVal = movePiecetoLoc(sel);
			break;
		case END_SELECT:
			break;
	}
	return retVal;
}

int Controller::selectingPiece(SELECT_STATES& sel){
	int ret = 0;
	if(chessBoard.getPieceColour(cursorL[SELECT_PIECE].h,cursorL[SELECT_PIECE].w) == player[turn]) {
		sel = SELECT_LOCATION;
		ret = SELECT_OK;
	} 
	else ret = INVALID_SELECT; //Invalid selection
	return ret;
}

int Controller::movePiecetoLoc(SELECT_STATES& sel){
	int ret = 0;
	if((ret = chessBoard.movePiece(cursorL[SELECT_LOCATION],cursorL[SELECT_PIECE])) > -1) {
		if(ret == PAWN_PROMOTION) gameState = PROMOTE;
		sel = END_SELECT;
	}
	else sel = SELECT_PIECE;
	return ret;
}

