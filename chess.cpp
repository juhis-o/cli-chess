#include "chess.h"

void chessPiece::moveEmptySpace(CursorLoc &newLoc, CursorLoc &oldLoc){
	board[newLoc.h][newLoc.w].swap(board[oldLoc.h][oldLoc.w]);
}

void chessPiece::moveOccupiedSpace(CursorLoc &newLoc, CursorLoc &oldLoc){
	board[newLoc.h][newLoc.w].swap(board[oldLoc.h][oldLoc.w]);
	board[oldLoc.h][oldLoc.w] = std::make_unique<emptyPiece>(board);
}

bool chessPiece::calcPath(CursorLoc &newLoc, CursorLoc &oldLoc){
	int8_t deltaY = newLoc.h - oldLoc.h;
    int8_t deltaX = newLoc.w - oldLoc.w;
    int8_t stepY = (deltaY > 0) ? 1 : (deltaY < 0) ? -1 : 0;
    int8_t stepX = (deltaX > 0) ? 1 : (deltaX < 0) ? -1 : 0;

    for (int8_t h = oldLoc.h + stepY, w = oldLoc.w + stepX; h != newLoc.h || w != newLoc.w; h += stepY, w += stepX) {
        if (board[h][w]->chessChar != ' ') {
            return false;
        }
    }
    return true;
}

void pawnPiece::checkSquares(int h, int w){
	bool colour = pieceColour != BLUE;
	int8_t dirH = colour ? 1 : -1;
	
	for(int8_t i = -1; i < 2; i+=2) {
		if((h+dirH) >= 0 && (h+dirH) < BOARD_SIZE && (w+i) >= 0 && (w+i) < BOARD_SIZE) {
			board[h+dirH][w+i]->threat[colour] = true;
		}
	}
}

int pawnPiece::move(CursorLoc &newLoc, CursorLoc &oldLoc){
	int8_t movementY = newLoc.h - oldLoc.h;
	int8_t movementX = newLoc.w - oldLoc.w;
	int ret = MOVE_OK;
	bool direction = pieceColour != BLUE;

	if(memcmp(&newLoc,&oldLoc,sizeof(newLoc)) == 0) return -MOVE_CANCEL;

	if (((direction && movementY > 0) || (!direction && movementY < 0)) && (movementX == 0)) {
		if(firstMove){
			if(abs(movementY) <= 2 && (calcPath(newLoc,oldLoc))) {
				firstMove = false;
				moveEmptySpace(newLoc, oldLoc);
			}
			else ret = -MOVE_NOT_VALID;
		}
		else {
			if(abs(movementY) == 1 && (board[newLoc.h][newLoc.w]->chessChar == ' ')) {
				moveEmptySpace(newLoc, oldLoc);
			}
			else ret = -MOVE_NOT_VALID;
		}
	}
	else if (((direction && movementY > 0) || (!direction && movementY < 0)) &&
	(abs(movementX) == 1 && abs(movementY) == 1) && (board[newLoc.h][newLoc.w]->chessChar != ' ')){
        if ((direction && board[newLoc.h][newLoc.w]->pieceColour == BLUE) ||
            (!direction && board[newLoc.h][newLoc.w]->pieceColour == RED)) {
				moveOccupiedSpace(newLoc, oldLoc);
			}
		else ret = -MOVE_NOT_VALID;
	}
	else ret = -MOVE_NOT_VALID;

	return ret;
}

void towerPiece::checkSquares(int h, int w){
	bool colour = pieceColour != BLUE;
	for(uint8_t t = 0; t < 4; t++) {
		int8_t i = 0, j = 0, dir = 0;
		bool piecePassed = false;
		switch (t) {
			case 0:	i = dir = 1;
				break;
			case 1: j = dir = 1;
				break;
			case 2: i = dir = -1;
				break;
			case 3: j = dir = -1;
				break;
			default:
				break;
		}
		for(;(h + i < BOARD_SIZE) && (w + j < BOARD_SIZE) && (h + i >= 0) && (w + j >= 0); t%2 == 0 ? i+=dir : j+=dir) {
			board[h+i][w+j]->threat[colour] = true;
			if(piecePassed || board[h+i][w+j]->chessChar != ' ') {
				if((board[h+i][w+j]->chessChar == 'K')) piecePassed = true;
				else break;
			}
		}
	}
}

int towerPiece::move(CursorLoc &newLoc, CursorLoc &oldLoc){
	bool colour = pieceColour != BLUE;
	int ret = MOVE_OK;

	if(memcmp(&newLoc,&oldLoc,sizeof(newLoc)) == 0) return -MOVE_CANCEL;

	if((newLoc.h == oldLoc.h) || (newLoc.w == oldLoc.w)){
		if(calcPath(newLoc,oldLoc)) {
			if(board[newLoc.h][newLoc.w]->chessChar == ' '){
				moveEmptySpace(newLoc, oldLoc);
			}
			else {
            	if ((colour && board[newLoc.h][newLoc.w]->pieceColour == BLUE) ||
                (!colour && board[newLoc.h][newLoc.w]->pieceColour == RED)) {
				moveOccupiedSpace(newLoc, oldLoc);
				}
				else ret = -CAPTURING_OWN_PIECE;
			}
		}
		else ret = -PIECE_ON_PATH;
	}
	else ret = -MOVE_NOT_VALID;

	return ret;
}

void bishopPiece::checkSquares(int h, int w){
	bool colour = pieceColour != BLUE;
	for(uint8_t t = 0; t < 4; t++) {
		bool piecePassed = false;
		int8_t i = 0, j = 0, dir1 = 0, dir2 = 0;
		switch (t) {
			case 0:	i = dir2 = j = dir1 = 1;
				break;
			case 1: i = dir2 = j = dir1 = -1;
				break;
			case 2: i = dir2 = -1; j = dir1 = 1;
				break;
			case 3: i = dir2 = 1; j = dir1 = -1;
				break;
			default:
				break;
		}
		for(;(h + i < BOARD_SIZE) && (w + j < BOARD_SIZE) && (h + i >= 0) && (w + j >= 0); j+=dir1, i+=dir2) {
			board[h+i][w+j]->threat[colour] = true;
			if(piecePassed || board[h+i][w+j]->chessChar != ' ') {
				if((board[h+i][w+j]->chessChar == 'K')) piecePassed = true;
				else break;
			}
		}
	}
}

int bishopPiece::move(CursorLoc &newLoc, CursorLoc &oldLoc){
	int8_t movementY = newLoc.h - oldLoc.h;
	int8_t movementX = newLoc.w - oldLoc.w;
	bool colour = pieceColour != BLUE;
	int ret = MOVE_OK;

	if(memcmp(&newLoc,&oldLoc,sizeof(newLoc)) == 0) return -MOVE_CANCEL;

	if(abs(movementX) == abs(movementY)) {
		if(calcPath(newLoc,oldLoc)) {
			if(board[newLoc.h][newLoc.w]->chessChar == ' '){
				moveEmptySpace(newLoc, oldLoc);
			}
			else {
            	if ((colour && board[newLoc.h][newLoc.w]->pieceColour == BLUE) ||
                (!colour && board[newLoc.h][newLoc.w]->pieceColour == RED)) {
				moveOccupiedSpace(newLoc, oldLoc);
				}
				else ret = -CAPTURING_OWN_PIECE;
			}
		}
		else ret = -PIECE_ON_PATH;
	}
	else ret = -MOVE_NOT_VALID;

	return ret;
}

void horsePiece::checkSquares(int h, int w){
	bool colour = pieceColour != BLUE;
	int8_t horseMovement[2] = {1,2};
	bool flip = false;

	for(int i = 0; i < 8; i++, !(i%2) ? horseMovement[0]*=-1 : horseMovement[1]*=-1, flip = !(i%4) ? !flip : flip) {
		if((h + horseMovement[flip] >= BOARD_SIZE) || (w + horseMovement[!flip] >= BOARD_SIZE) || (h + horseMovement[flip] < 0) || (w + horseMovement[!flip] < 0)) continue;
		board[h+horseMovement[flip]][w+horseMovement[!flip]]->threat[colour] = true;
	}
}

int horsePiece::move(CursorLoc &newLoc, CursorLoc &oldLoc){
	int8_t movementY = newLoc.h - oldLoc.h;
	int8_t movementX = newLoc.w - oldLoc.w;
	bool colour = pieceColour != BLUE;
	int ret = MOVE_OK;

	if(memcmp(&newLoc,&oldLoc,sizeof(newLoc)) == 0) return -MOVE_CANCEL;

	if((abs(movementX) == 1 && abs(movementY) == 2) || (abs(movementX) == 2 && abs(movementY) == 1)){
		if(board[newLoc.h][newLoc.w]->chessChar == ' '){
			moveEmptySpace(newLoc, oldLoc);
		}
		else {
            if ((colour && board[newLoc.h][newLoc.w]->pieceColour == BLUE) ||
                (!colour && board[newLoc.h][newLoc.w]->pieceColour == RED)) {
				moveOccupiedSpace(newLoc, oldLoc);
				}
			else ret = -CAPTURING_OWN_PIECE;
		}
	}
	else ret = -MOVE_NOT_VALID;
	return ret;
}

void queenPiece::checkSquares(int h, int w){
	bool colour = pieceColour != BLUE;
	for(uint8_t t = 0; t < 8; t++) {
		int8_t i = 0, j = 0, dir1 = 0, dir2 = 0;
		bool piecePassed = false;
		switch (t) {
			case 0:	i = dir2 = j = dir1 = 1;
				break;
			case 1: i = dir2 = j = dir1 = -1;
				break;
			case 2: i = dir2 = -1; j = dir1 = 1;
				break;
			case 3: i = dir2 = 1; j = dir1 = -1;
				break;
			case 4:	i = dir1 = 1;
				break;
			case 5: j = dir1 = 1;
				break;
			case 6: i = dir1 = -1;
				break;
			case 7: j = dir1 = -1;
				break;
			default:
				break;
		}											
		for(;(h + i < BOARD_SIZE) && (w + j < BOARD_SIZE) && (h + i >= 0) && (w + j >= 0); t<4 ? j+=dir1, i+=dir2 : !(t%2) ? i+=dir1 : j+=dir1) {
			board[h+i][w+j]->threat[colour] = true;
			if(piecePassed || board[h+i][w+j]->chessChar != ' ') {
				if((board[h+i][w+j]->chessChar == 'K')) piecePassed = true;
				else break;
			}
		}
	}
}

int queenPiece::move(CursorLoc &newLoc, CursorLoc &oldLoc){
	int8_t movementY = newLoc.h - oldLoc.h;
	int8_t movementX = newLoc.w - oldLoc.w;
	bool colour = pieceColour != BLUE;
	int ret = MOVE_OK;

	if(memcmp(&newLoc,&oldLoc,sizeof(newLoc)) == 0) return -MOVE_CANCEL;

	if((abs(movementX) == abs(movementY)) || ((newLoc.h == oldLoc.h) || (newLoc.w == oldLoc.w))) {
		if(calcPath(newLoc,oldLoc)) {
			if(board[newLoc.h][newLoc.w]->chessChar == ' '){
				moveEmptySpace(newLoc, oldLoc);
			}
			else {
            	if ((colour && board[newLoc.h][newLoc.w]->pieceColour == BLUE) ||
                (!colour && board[newLoc.h][newLoc.w]->pieceColour == RED)) {
				moveOccupiedSpace(newLoc, oldLoc);
				}
				else ret = -CAPTURING_OWN_PIECE;
			}
		}
		else ret = -PIECE_ON_PATH;
	}
	else ret = -MOVE_NOT_VALID;

	return ret;
}

void kingPiece::checkSquares(int h, int w){
	bool colour = pieceColour != BLUE;
	int8_t kingMovement[2] = {1,1};
	bool flip = false;

	for(int i = 0; i < 8; i++, !(i % 2) ? kingMovement[0]*=-1 : kingMovement[1]*=-1, kingMovement[0] = !(i%4) ? 0 : kingMovement[0], flip = (i % 6) ? flip : !flip ) {
		if((h + kingMovement[flip] >= BOARD_SIZE) || (w + kingMovement[!flip] >= BOARD_SIZE) || (h + kingMovement[flip] < 0) || (w + kingMovement[!flip] < 0)) continue;
		board[h+kingMovement[flip]][w+kingMovement[!flip]]->threat[colour] = true;
	}
}

int kingPiece::move(CursorLoc &newLoc, CursorLoc &oldLoc){
	int8_t movementY = newLoc.h - oldLoc.h;
	int8_t movementX = newLoc.w - oldLoc.w;
	bool colour = pieceColour != BLUE;
	int ret = MOVE_OK;

	if(memcmp(&newLoc,&oldLoc,sizeof(newLoc)) == 0) return -MOVE_CANCEL;
	
	if(abs(movementX) == 1 || abs(movementY) == 1) {
		if(board[newLoc.h][newLoc.w]->chessChar == ' '){
			if(!board[newLoc.h][newLoc.w]->threat[!colour]) moveEmptySpace(newLoc, oldLoc);
			else ret = -THREAT;
		}
		else {
            if ((colour && board[newLoc.h][newLoc.w]->pieceColour == BLUE) ||
                (!colour && board[newLoc.h][newLoc.w]->pieceColour == RED)) {
				if(!board[newLoc.h][newLoc.w]->threat[!colour]) moveOccupiedSpace(newLoc, oldLoc);
				else ret = -THREAT;
			}
			else ret = -CAPTURING_OWN_PIECE;
		}
	}
	else ret = -MOVE_NOT_VALID;

	return ret;
}

ChessBoard::ChessBoard() {
	uint8_t unit_colour = 1;
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
	updateThreatSquares(false);
}

void ChessBoard::FillRow(int row, uint8_t& unit_colour, std::vector<std::unique_ptr<chessPiece>>&Board) {
	switch(row) {
		case BACKROW:
			for (int i = 0; i < BOARD_SIZE; i++) {
    			if (i == 0 || i == 7) {
        			Board[i] = std::make_unique<towerPiece>(board);
    			} else if (i == 1 || i == 6) {
    			    Board[i] = std::make_unique<horsePiece>(board);
    			} else if (i == 2 || i == 5) {
        			Board[i] = std::make_unique<bishopPiece>(board);
    			} else if (i == 3) {
       				Board[i] = std::make_unique<queenPiece>(board);
    			} else {
        			Board[i] = std::make_unique<kingPiece>(board);
    			}
				Board[i]->pieceColour = unit_colour;
			}
			break;

		case FRONTROW:
			for(int i = 0; i < BOARD_SIZE; i++){
				Board[i] = std::make_unique<pawnPiece>(board);
				Board[i]->pieceColour = unit_colour;
			}
			break;

		default:
			for(int i = 0; i < BOARD_SIZE; i++){
				Board[i] = std::make_unique<emptyPiece>(board);
				Board[i]->pieceColour = 0;
			}
			break;
	}
}

void ChessBoard::updateThreatSquares(bool reset) {
	if(reset) {
		for (uint8_t i = 0; i < BOARD_SIZE; i++){
			for(uint8_t j = 0; j < BOARD_SIZE; j++){
				board[i][j]->threat[0] = false;
				board[i][j]->threat[1] = false;
			}
		}
	}

    for (uint8_t i = 0; i < BOARD_SIZE; i++){
		for(uint8_t j = 0; j < BOARD_SIZE; j++){
			if(board[i][j]->chessChar != ' ') board[i][j]->checkSquares(i,j);
		}
	}
}

/*
void ChessBoard::FillRow(int row, uint8_t& unit_colour, std::vector<std::unique_ptr<chessPiece>>&Board) {
	switch(row) {
		case BACKROW:

			for(int i = 0; i < 3; i++){
				Board[i] = std::make_unique<emptyPiece>(board);
			}
       		Board[3] = std::make_unique<kingPiece>(board);
			Board[3]->pieceColour = unit_colour;
			for(int i = 4; i < 8; i++){
				Board[i] = std::make_unique<emptyPiece>(board);
			}
			break;
		case FRONTROW:
			for(int i = 0; i < 3; i++){
				Board[i] = std::make_unique<emptyPiece>(board);
			}
			Board[3] = std::make_unique<queenPiece>(board);
			Board[3]->pieceColour = unit_colour;
			for(int i = 4; i < 8; i++){
				Board[i] = std::make_unique<emptyPiece>(board);
			}
			break;

		default:
			for(int i = 0; i < BOARD_SIZE; i++){
				Board[i] = std::make_unique<emptyPiece>(board);
			}
			break;
	}
}*/
