#include "chess.h"

void chessPiece::moveEmptySpace(CursorLoc &newLoc, CursorLoc &oldLoc, std::vector<std::vector<std::unique_ptr<chessPiece>>>& board){
	bool temp[2] = {board[oldLoc.h][oldLoc.w]->bgColour,board[newLoc.h][newLoc.w]->bgColour};
	board[newLoc.h][newLoc.w].swap(board[oldLoc.h][oldLoc.w]);
	board[oldLoc.h][oldLoc.w]->bgColour = temp[0];
	board[newLoc.h][newLoc.w]->bgColour = temp[1];
}

int pawnPiece::move(CursorLoc &newLoc, CursorLoc &oldLoc, std::vector<std::vector<std::unique_ptr<chessPiece>>>& board){
	
	int movementY = newLoc.h - oldLoc.h;
	int ret = 0;
	bool direction = (pieceColour == BLUE) ? false : true;

	if (!(board[newLoc.h][newLoc.w]->isEmpty)) {
		return -1;
	}

	if ((direction && movementY > 0) || (!direction && movementY < 0)) {
		if(firstMove){
			if(abs(movementY) <= 2) {
				firstMove = false;
				moveEmptySpace(newLoc, oldLoc, board);
				ret = 10;
			}
		else ret = -1;
		}
	else {
		if(abs(movementY) == 1) {
			moveEmptySpace(newLoc, oldLoc, board);
			ret = 10;
		}
		else ret = -1;
		}
	}
	else ret = -1;

	return ret;
}

int horsePiece::move(CursorLoc &newLoc, CursorLoc &oldLoc, std::vector<std::vector<std::unique_ptr<chessPiece>>>& board){

	int movement = newLoc.h - oldLoc.h;
	int ret = 1;

	bool temp[2] = {board[oldLoc.h][oldLoc.w]->bgColour,board[newLoc.h][newLoc.w]->bgColour};
	board[newLoc.h][newLoc.w].swap(board[oldLoc.h][oldLoc.w]);
	board[oldLoc.h][oldLoc.w]->bgColour = temp[0];
	board[newLoc.h][newLoc.w]->bgColour = temp[1];
	return ret;
}

int towerPiece::move(CursorLoc &newLoc, CursorLoc &oldLoc, std::vector<std::vector<std::unique_ptr<chessPiece>>>& board){

	int movement = newLoc.h - oldLoc.h;
	int ret = 2;

	bool temp[2] = {board[oldLoc.h][oldLoc.w]->bgColour,board[newLoc.h][newLoc.w]->bgColour};
	board[newLoc.h][newLoc.w].swap(board[oldLoc.h][oldLoc.w]);
	board[oldLoc.h][oldLoc.w]->bgColour = temp[0];
	board[newLoc.h][newLoc.w]->bgColour = temp[1];
	return ret;
}

int bishopPiece::move(CursorLoc &newLoc, CursorLoc &oldLoc, std::vector<std::vector<std::unique_ptr<chessPiece>>>& board){

	int movement = newLoc.h - oldLoc.h;
	int ret = 3;

	bool temp[2] = {board[oldLoc.h][oldLoc.w]->bgColour,board[newLoc.h][newLoc.w]->bgColour};
	board[newLoc.h][newLoc.w].swap(board[oldLoc.h][oldLoc.w]);
	board[oldLoc.h][oldLoc.w]->bgColour = temp[0];
	board[newLoc.h][newLoc.w]->bgColour = temp[1];
	return ret;
}

int queenPiece::move(CursorLoc &newLoc, CursorLoc &oldLoc, std::vector<std::vector<std::unique_ptr<chessPiece>>>& board){

	int movement = newLoc.h - oldLoc.h;
	int ret = 4;

	bool temp[2] = {board[oldLoc.h][oldLoc.w]->bgColour,board[newLoc.h][newLoc.w]->bgColour};
	board[newLoc.h][newLoc.w].swap(board[oldLoc.h][oldLoc.w]);
	board[oldLoc.h][oldLoc.w]->bgColour = temp[0];
	board[newLoc.h][newLoc.w]->bgColour = temp[1];
	return ret;
}

int kingPiece::move(CursorLoc &newLoc, CursorLoc &oldLoc, std::vector<std::vector<std::unique_ptr<chessPiece>>>& board){

	int movement = newLoc.h - oldLoc.h;
	int ret = 5;
	bool temp[2] = {board[oldLoc.h][oldLoc.w]->bgColour,board[newLoc.h][newLoc.w]->bgColour};
	board[newLoc.h][newLoc.w].swap(board[oldLoc.h][oldLoc.w]);
	board[oldLoc.h][oldLoc.w]->bgColour = temp[0];
	board[newLoc.h][newLoc.w]->bgColour = temp[1];
	return ret;
}

ChessBoard::ChessBoard() {
	bool background_colour = 1;
	int unit_colour = 1;
	board.resize(BOARD_SIZE);
    for (int i = 0; i < BOARD_SIZE; i++){
		board[i].resize(BOARD_SIZE);
		switch(i) {
			case 0: case 7:
				FillRow(BACKROW, background_colour,unit_colour, board[i]);
				break;
			case 1: case 6:
				FillRow(FRONTROW, background_colour,unit_colour, board[i]);
				break;
			default:
				FillRow(EMPTYROW, background_colour,unit_colour, board[i]);
				break;
		}
		if(i == 1) unit_colour+=2;
	    }
}

void ChessBoard::FillRow(int row, bool& bgColour, int& unit_colour, std::vector<std::unique_ptr<chessPiece>>&Board) {

	switch(row) {
		case BACKROW:
			for (int i = 0; i < 8; i++) {
    			if (i == 0 || i == 7) {
        			Board[i] = std::make_unique<towerPiece>();
    			} else if (i == 1 || i == 6) {
    			    Board[i] = std::make_unique<horsePiece>();
    			} else if (i == 2 || i == 5) {
        			Board[i] = std::make_unique<bishopPiece>();
    			} else if (i == 3) {
       				Board[i] = std::make_unique<queenPiece>();
    			} else {
        			Board[i] = std::make_unique<kingPiece>();
    			}
				bgColour = !bgColour;
				Board[i]->bgColour = bgColour;
				Board[i]->pieceColour = unit_colour;
			}
			bgColour = !bgColour;
			break;
		case FRONTROW:
			for(int i = 0; i < 8; i++){
				Board[i] = std::make_unique<pawnPiece>();
				bgColour = !bgColour;
				Board[i]->bgColour = bgColour;
				Board[i]->pieceColour = unit_colour;
			}
    		bgColour = !bgColour;
			break;
		default:
			for(int i = 0; i < 8; i++){
				Board[i] = std::make_unique<emptySquare>();
				bgColour = !bgColour;
				Board[i]->bgColour = bgColour;
				Board[i]->pieceColour = unit_colour;
			}
    		bgColour = !bgColour;
			break;
	}
}