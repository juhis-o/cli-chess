#include "chess.h"

int chessPiece::moveEmptySpace(CursorLoc &newLoc, CursorLoc &oldLoc, std::vector<std::vector<std::unique_ptr<chessPiece>>>& board){
	board[newLoc.h][newLoc.w].swap(board[oldLoc.h][oldLoc.w]);
	return 10;
}

int chessPiece::moveOccupiedSpace(CursorLoc &newLoc, CursorLoc &oldLoc, std::vector<std::vector<std::unique_ptr<chessPiece>>>& board){
	board[newLoc.h][newLoc.w].swap(board[oldLoc.h][oldLoc.w]);
	board[oldLoc.h][oldLoc.w] = nullptr;
	return 11;
}

bool chessPiece::calcPath(CursorLoc &newLoc, CursorLoc &oldLoc, std::vector<std::vector<std::unique_ptr<chessPiece>>>& board){
	int deltaY = newLoc.h - oldLoc.h;
    int deltaX = newLoc.w - oldLoc.w;
    int stepY = (deltaY > 0) ? 1 : (deltaY < 0) ? -1 : 0;
    int stepX = (deltaX > 0) ? 1 : (deltaX < 0) ? -1 : 0;

    for (int h = oldLoc.h + stepY, w = oldLoc.w + stepX; h != newLoc.h || w != newLoc.w; h += stepY, w += stepX) {
        if (board[h][w] != nullptr) {
            return false;
        }
    }
    return true;
}

int pawnPiece::move(CursorLoc &newLoc, CursorLoc &oldLoc, std::vector<std::vector<std::unique_ptr<chessPiece>>>& board){
	int movementY = newLoc.h - oldLoc.h;
	int movementX = newLoc.w - oldLoc.w;
	int ret = 0;
	bool direction = (pieceColour == BLUE) ? false : true;

	if((newLoc.h == oldLoc.h) && (newLoc.w == oldLoc.w)) return -6;

	if (((direction && movementY > 0) || (!direction && movementY < 0)) && (movementX == 0)) {
		if(firstMove){
			if(abs(movementY) <= 2 && (calcPath(newLoc,oldLoc,board))) {
				firstMove = false;
				ret = moveEmptySpace(newLoc, oldLoc, board);
			}
			else ret = -1;
		}
		else {
			if(abs(movementY) == 1 && (board[newLoc.h][newLoc.w] == nullptr)) {
				ret = moveEmptySpace(newLoc, oldLoc, board);
			}
			else ret = -1;
		}
	}
	else if (((direction && movementY > 0) || (!direction && movementY < 0)) &&
	(abs(movementX) == 1 && abs(movementY) == 1) && (board[newLoc.h][newLoc.w] != nullptr)){
        if ((direction && board[newLoc.h][newLoc.w]->pieceColour == BLUE) ||
            (!direction && board[newLoc.h][newLoc.w]->pieceColour == RED)) {
				ret = moveOccupiedSpace(newLoc, oldLoc, board);
				}
		else ret = -10;
	}
	else ret = -1;

	return ret;
}

int towerPiece::move(CursorLoc &newLoc, CursorLoc &oldLoc, std::vector<std::vector<std::unique_ptr<chessPiece>>>& board){
	int movementY = newLoc.h - oldLoc.h;
	int movementX = newLoc.w - oldLoc.w;
	bool colour = (pieceColour == BLUE) ? false : true;
	int ret = 2;

	if((newLoc.h == oldLoc.h) && (newLoc.w == oldLoc.w)) return -6;

	if((newLoc.h == oldLoc.h) || (newLoc.w == oldLoc.w)){
		if(calcPath(newLoc,oldLoc,board)) {
			if(board[newLoc.h][newLoc.w] == nullptr){
				ret = moveEmptySpace(newLoc, oldLoc, board);
			}
			else {
            	if ((colour && board[newLoc.h][newLoc.w]->pieceColour == BLUE) ||
                (!colour && board[newLoc.h][newLoc.w]->pieceColour == RED)) {
				ret = moveOccupiedSpace(newLoc, oldLoc, board);
				}
				else ret = -3;
			}
		}
		else ret = -2;
	}
	else ret = -1;

	return ret;
}

int bishopPiece::move(CursorLoc &newLoc, CursorLoc &oldLoc, std::vector<std::vector<std::unique_ptr<chessPiece>>>& board){
	int movementY = newLoc.h - oldLoc.h;
	int movementX = newLoc.w - oldLoc.w;
	bool colour = (pieceColour == BLUE) ? false : true;
	int ret = 3;

	if((newLoc.h == oldLoc.h) && (newLoc.w == oldLoc.w)) return -6;

	if(abs(movementX) == abs(movementY)) {
		if(calcPath(newLoc,oldLoc,board)) {
			if(board[newLoc.h][newLoc.w] == nullptr){
				ret = moveEmptySpace(newLoc, oldLoc, board);
			}
			else {
            	if ((colour && board[newLoc.h][newLoc.w]->pieceColour == BLUE) ||
                (!colour && board[newLoc.h][newLoc.w]->pieceColour == RED)) {
				ret = moveOccupiedSpace(newLoc, oldLoc, board);
				}
				else ret = -3;
			}
		}
		else ret = -2;
	}
	else ret = -1;

	return ret;
}

int horsePiece::move(CursorLoc &newLoc, CursorLoc &oldLoc, std::vector<std::vector<std::unique_ptr<chessPiece>>>& board){
	int movementY = newLoc.h - oldLoc.h;
	int movementX = newLoc.w - oldLoc.w;
	bool colour = (pieceColour == BLUE) ? false : true;
	int ret = 1;

	if((newLoc.h == oldLoc.h) && (newLoc.w == oldLoc.w)) return -6;

	if((abs(movementX) == 1 && abs(movementY) == 2) || (abs(movementX) == 2 && abs(movementY) == 1)){
		if(board[newLoc.h][newLoc.w] == nullptr){
			ret = moveEmptySpace(newLoc, oldLoc, board);
		}
		else {
            if ((colour && board[newLoc.h][newLoc.w]->pieceColour == BLUE) ||
                (!colour && board[newLoc.h][newLoc.w]->pieceColour == RED)) {
				ret = moveOccupiedSpace(newLoc, oldLoc, board);
				}
			else ret = -3;
		}
	}
	else ret = -1;

	return ret;
}

int queenPiece::move(CursorLoc &newLoc, CursorLoc &oldLoc, std::vector<std::vector<std::unique_ptr<chessPiece>>>& board){
	int movementY = newLoc.h - oldLoc.h;
	int movementX = newLoc.w - oldLoc.w;
	bool colour = (pieceColour == BLUE) ? false : true;
	int ret = 4;

	if((newLoc.h == oldLoc.h) && (newLoc.w == oldLoc.w)) return -6;

	if((abs(movementX) == abs(movementY)) || ((newLoc.h == oldLoc.h) || (newLoc.w == oldLoc.w))) {
		if(calcPath(newLoc,oldLoc,board)) {
			if(board[newLoc.h][newLoc.w] == nullptr){
				ret = moveEmptySpace(newLoc, oldLoc, board);
			}
			else {
            	if ((colour && board[newLoc.h][newLoc.w]->pieceColour == BLUE) ||
                (!colour && board[newLoc.h][newLoc.w]->pieceColour == RED)) {
				ret = moveOccupiedSpace(newLoc, oldLoc, board);
				}
				else ret = -3;
			}
		}
		else ret = -2;
	}
	else ret = -1;


	return ret;
}

int kingPiece::move(CursorLoc &newLoc, CursorLoc &oldLoc, std::vector<std::vector<std::unique_ptr<chessPiece>>>& board){
	int movementY = newLoc.h - oldLoc.h;
	int movementX = newLoc.w - oldLoc.w;
	bool colour = (pieceColour == BLUE) ? false : true;
	int ret = 5;

	if((newLoc.h == oldLoc.h) && (newLoc.w == oldLoc.w)) return -6;

	if(abs(movementX) == 1 || abs(movementY) == 1) {
		if(board[newLoc.h][newLoc.w] == nullptr){
			ret = moveEmptySpace(newLoc, oldLoc, board);
		}
		else {
            if ((colour && board[newLoc.h][newLoc.w]->pieceColour == BLUE) ||
                (!colour && board[newLoc.h][newLoc.w]->pieceColour == RED)) {
				ret = moveOccupiedSpace(newLoc, oldLoc, board);
			}
			else ret = -3;
		}
	}

	return ret;
}

ChessBoard::ChessBoard() {
	int unit_colour = 1;
	board.resize(BOARD_SIZE);
    for (int i = 0; i < BOARD_SIZE; i++){
		board[i].resize(BOARD_SIZE);
		switch(i) {
			case 0: case 7:
				FillRow(BACKROW, unit_colour, board[i]);
				break;
			case 1: case 6:
				FillRow(FRONTROW, unit_colour, board[i]);
				break;
			default:
				FillRow(EMPTYROW,unit_colour, board[i]);
				break;
		}
		if(i == 1) unit_colour+=2;
	    }
}

void ChessBoard::FillRow(int row, int& unit_colour, std::vector<std::unique_ptr<chessPiece>>&Board) {
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
				Board[i]->pieceColour = unit_colour;
			}
			break;
		case FRONTROW:
			for(int i = 0; i < 8; i++){
				Board[i] = std::make_unique<pawnPiece>();
				Board[i]->pieceColour = unit_colour;
			}
			break;
		default:
			for(int i = 0; i < 8; i++){
				Board[i] = nullptr;
			}
			break;
	}
}