#include "controller.h"

Controller::Controller(){
    bool quit = false;
    chessPiece_retVals retVal = SELECT_OK;
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

chessPiece_retVals Controller::selectState(bool& quit){
    chessPiece_retVals retVal = SELECT_OK;
    for(SELECT_STATES selectState = SELECT_PIECE; selectState < END_SELECT;){
        if((quit = userI.Select(cursorL[selectState])))
            break;
        if(inBounds(cursorL[selectState].h,cursorL[selectState].w))
            retVal = selection(selectState);
        else
            retVal = INVALID_SELECT; //Out of bounds
        if(retVal < MOVE_OK)
            userI.printSelectState(chessBoard,retVal);
    }
    return retVal;
}

chessPiece_retVals Controller::selection(SELECT_STATES& sel){
    chessPiece_retVals retVal = SELECT_OK;
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

chessPiece_retVals Controller::selectingPiece(SELECT_STATES& sel){
    chessPiece_retVals ret = SELECT_OK;
    if(chessBoard.getPieceColour(cursorL[SELECT_PIECE].h,cursorL[SELECT_PIECE].w) == chessBoard.playerColours[turn]) {
        sel = SELECT_LOCATION;
        ret = SELECT_OK;
    } 
    else
        ret = INVALID_SELECT; //Invalid selection
    return ret;
}

chessPiece_retVals Controller::movePiecetoLoc(SELECT_STATES& sel){
    chessPiece_retVals ret = SELECT_OK;
    if((ret = chessBoard.movePiece(cursorL[SELECT_LOCATION],cursorL[SELECT_PIECE], turn)) > -1) {
        if(ret == PAWN_PROMOTION)
            gameState = PROMOTE;
        sel = END_SELECT;
    }
    else
        sel = SELECT_PIECE;
    return ret;
}

