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

void pawnPiece::checkSquares(int h, int w, std::vector<ThreatLoc>& loc){
	bool colour = pieceColour != BLUE;
	int8_t dirH = colour ? 1 : -1;

	for(int i = dirH, target = firstMove+abs(dirH)+1; abs(i) < target; i+=dirH){
		if(board[h+i][w]->chessChar == ' ') board[h+i][w]->pawnPath[colour] = true;
		else break;
	}

	for(int8_t i = -1; i < 2; i+=2) {
		if((h+dirH) >= 0 && (h+dirH) < BOARD_SIZE && (w+i) >= 0 && (w+i) < BOARD_SIZE) {
			board[h+dirH][w+i]->piecePath[colour] = 2;
			if((board[h+dirH][w+i]->chessChar == 'K') &&
			((colour && board[h+dirH][w+i]->pieceColour == BLUE) ||
			(!colour && board[h+dirH][w+i]->pieceColour == RED))) {
				ThreatLoc newThreat{(int8_t)h,(int8_t)w,false};
				loc.push_back(newThreat);
			}
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
	else if (((direction && movementY > 0) ||
	(!direction && movementY < 0)) &&
	(abs(movementX) == 1 && abs(movementY) == 1) && 
	(board[newLoc.h][newLoc.w]->chessChar != ' ')){
        if ((direction && board[newLoc.h][newLoc.w]->pieceColour == BLUE) ||
            (!direction && board[newLoc.h][newLoc.w]->pieceColour == RED)) {
				moveOccupiedSpace(newLoc, oldLoc);
			}
		else ret = -MOVE_NOT_VALID;
	}
	else ret = -MOVE_NOT_VALID;
	return ret;
}

void towerPiece::checkSquares(int h, int w, std::vector<ThreatLoc>& loc){
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
			if(!board[h+i][w+j]->piecePath[colour])board[h+i][w+j]->piecePath[colour] = true;
			if(piecePassed || board[h+i][w+j]->chessChar != ' ') {
				if((board[h+i][w+j]->chessChar == 'K')) {
					piecePassed = true;
					if((colour && board[h+i][w+j]->pieceColour == BLUE) ||
					(!colour && board[h+i][w+j]->pieceColour == RED)) {
						ThreatLoc newThreat{(int8_t)h,(int8_t)w,false};
						loc.push_back(newThreat);
					}
				}
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

void bishopPiece::checkSquares(int h, int w, std::vector<ThreatLoc>& loc){
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
			if(!board[h+i][w+j]->piecePath[colour]) board[h+i][w+j]->piecePath[colour] = true;
			if(piecePassed || board[h+i][w+j]->chessChar != ' ') {
				if((board[h+i][w+j]->chessChar == 'K')) {
					piecePassed = true;
					if((colour && board[h+i][w+j]->pieceColour == BLUE) || (!colour && board[h+i][w+j]->pieceColour == RED)){
						ThreatLoc newThreat{(int8_t)h,(int8_t)w,false};
						loc.push_back(newThreat);
					}
				}
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

void horsePiece::checkSquares(int h, int w, std::vector<ThreatLoc>& loc){
	bool colour = pieceColour != BLUE;
	int8_t horseMovement[2] = {1,2};
	bool flip = false;
	for(int i = 0; i < 8; i++, !(i%2) ? horseMovement[0]*=-1 : horseMovement[1]*=-1, flip = !(i%4) ? !flip : flip) {
		if((h + horseMovement[flip] >= BOARD_SIZE) ||
		(w + horseMovement[!flip] >= BOARD_SIZE) ||
		(h + horseMovement[flip] < 0) ||
		(w + horseMovement[!flip] < 0)) continue;
		if(!board[h+horseMovement[flip]][w+horseMovement[!flip]]->piecePath[colour])board[h+horseMovement[flip]][w+horseMovement[!flip]]->piecePath[colour] = true;
		if((board[h+horseMovement[flip]][w+horseMovement[!flip]]->chessChar == 'K')) {
			if((colour && board[h+horseMovement[flip]][w+horseMovement[!flip]]->pieceColour == BLUE) ||
			(!colour && board[h+horseMovement[flip]][w+horseMovement[!flip]]->pieceColour == RED)){
				ThreatLoc newThreat{(int8_t)h,(int8_t)w,true};
				loc.push_back(newThreat);
			}
		}
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

void queenPiece::checkSquares(int h, int w, std::vector<ThreatLoc>& loc){
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
			if(!board[h+i][w+j]->piecePath[colour]) board[h+i][w+j]->piecePath[colour] = true;
			if(piecePassed || board[h+i][w+j]->chessChar != ' ') {
				if((board[h+i][w+j]->chessChar == 'K')) {
					if((colour && board[h+i][w+j]->pieceColour == BLUE) ||
					(!colour && board[h+i][w+j]->pieceColour == RED)) {
						ThreatLoc newThreat{(int8_t)h,(int8_t)w,false};
						loc.push_back(newThreat);
					}
					piecePassed = true;
				}
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

void kingPiece::checkSquares(int h, int w, std::vector<ThreatLoc>& loc){
	(void)loc;
	bool colour = pieceColour != BLUE;
	bool flip = false;
	int8_t kingMovement[2] = {1,1};
	for(int i = 0; i < 8; i++, !(i % 2) ? kingMovement[0]*=-1 : kingMovement[1]*=-1, kingMovement[0] = !(i%4) ? 0 : kingMovement[0], flip = (i % 6) ? flip : !flip ) {
		if((h + kingMovement[flip] >= BOARD_SIZE) ||
		(w + kingMovement[!flip] >= BOARD_SIZE) ||
		(h + kingMovement[flip] < 0) ||
		(w + kingMovement[!flip] < 0)) continue;
		if(!board[h+kingMovement[flip]][w+kingMovement[!flip]]->piecePath[colour])board[h+kingMovement[flip]][w+kingMovement[!flip]]->piecePath[colour] = 3; //Kings threat squares
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
			if(!board[newLoc.h][newLoc.w]->piecePath[!colour]) moveEmptySpace(newLoc, oldLoc);
			else ret = -THREAT;
		}
		else {
            if ((colour && board[newLoc.h][newLoc.w]->pieceColour == BLUE) ||
                (!colour && board[newLoc.h][newLoc.w]->pieceColour == RED)) {
				if(!board[newLoc.h][newLoc.w]->piecePath[!colour]) moveOccupiedSpace(newLoc, oldLoc);
				else ret = -THREAT;
			}
			else ret = -CAPTURING_OWN_PIECE;
		}
	}
	else ret = -MOVE_NOT_VALID;

	return ret;
}

int8_t ChessBoard::checkmate(bool playerTurn){
	bool colour = board[KingLoc[playerTurn].h][KingLoc[playerTurn].w]->pieceColour != BLUE;
	int8_t ret = 0;
	if(!board[KingLoc[playerTurn].h][KingLoc[playerTurn].w]->piecePath[!colour]) return ret; //No piece is threatening king
	else ret = -1;
	if(canKingMove(KingLoc[playerTurn])) return ret; //King can move out of threat square
	if(threatPath.empty()) {
		file << "threat empty\n";
		return false;
	}
	else {
		file << "items in threatpath vector\n";
		for (auto& x : threatPath) {
			if(findOverlaps(KingLoc[playerTurn], x)) return ret; //Threatening piece can be captured or other piece can move in its way
		}
	}

	return true;
}


bool ChessBoard::canKingMove(CursorLoc& KingLoc){
	bool colour = board[KingLoc.h][KingLoc.w]->pieceColour != BLUE;
	bool flip = false;
	bool canMove = false;
	int8_t kingMovement[2] = {1,1};

//	file << (int)KingLoc.h << " " << (int)KingLoc.w << " " << "\n";
	
	for(int i = 0; i < 8; i++, !(i % 2) ? kingMovement[0]*=-1 : kingMovement[1]*=-1, kingMovement[0] = !(i%4) ? 0 : kingMovement[0], flip = (i % 6) ? flip : !flip ) {
		if((KingLoc.h + kingMovement[flip] >= BOARD_SIZE) ||
		(KingLoc.w + kingMovement[!flip] >= BOARD_SIZE) ||
		(KingLoc.h + kingMovement[flip] < 0) ||
		(KingLoc.w + kingMovement[!flip] < 0) ||
		(board[KingLoc.h+kingMovement[flip]][KingLoc.w+kingMovement[!flip]]->pieceColour == board[KingLoc.h][KingLoc.w]->pieceColour)) continue;
		file << int(KingLoc.h+kingMovement[flip]) << " " << int(KingLoc.w+kingMovement[!flip]) << " " << "\n";
		if(!board[KingLoc.h+kingMovement[flip]][KingLoc.w+kingMovement[!flip]]->piecePath[!colour]){
			canMove = true;
			break;
	}
	}
	file << (int)canMove << "\n";
	return canMove;
}

bool ChessBoard::findOverlaps(CursorLoc &kingLoc, ThreatLoc &threatPiece){
    int stepY = (threatPiece.h > kingLoc.h) ? -1 : (threatPiece.h < kingLoc.h) ? 1 : 0;
    int stepX = (threatPiece.w > kingLoc.w) ? -1 : (threatPiece.w < kingLoc.w) ? 1 : 0;
	int loop = 0;
    bool colour = board[kingLoc.h][kingLoc.w]->pieceColour != BLUE;
	file << "Th H: " << (int)threatPiece.h << " Th W: " << (int)threatPiece.w << " Ki H: " << (int)kingLoc.h << " Ki W: " << (int)kingLoc.w << " StepY: " << stepY << " stepX: "  << stepX  << " Colour: " << (int)colour << " " << "\n";
    if(!threatPiece.horsePiece) {
		if(board[threatPiece.h][threatPiece.w]->piecePath[colour]) {
			file << "true " << "\n";
			return true;
		}
        for (int h = threatPiece.h, w = threatPiece.w; h != kingLoc.h || w != kingLoc.w; h += stepY, w += stepX, loop++) {
            if (h < 0 || h >= BOARD_SIZE || w < 0 || w >= BOARD_SIZE) {
				file << "break " << "\n";
				break;
			} 
            file << (int)board[h][w]->piecePath[0] << " " << (int)board[h][w]->piecePath[1] << " " << (int)board[h][w]->pawnPath[colour] << " " << loop << "\n";
            if ((board[h][w]->piecePath[0] == 1 && board[h][w]->piecePath[1] == 1) || (board[h][w]->pawnPath[colour])) {
                file << "true " << "\n";
                return true;
            }
			file << h << " " << w << " " << loop << "\n";
        }
    } else {
        file << threatPiece.h << " " << threatPiece.w << " " << "\n";
        if((board[threatPiece.h][threatPiece.w]->piecePath[!colour]) || (board[threatPiece.h][threatPiece.w]->piecePath[colour] && board[threatPiece.h][threatPiece.w]->pawnPath[!colour])) {
			file << "true " << "\n";
			return true;
		}
    }
	file << "false " << "\n";
    return false;
}

ChessBoard::ChessBoard() {
	uint8_t unit_colour = 1;
	KingLoc[0] = KingLoc[1] = {0,0};
	file.open("output.txt");
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
			}
			break;
	}
}
void ChessBoard::updateThreatSquares(bool reset) {
	if(reset) {
		for (uint8_t i = 0; i < BOARD_SIZE; i++){
			for(uint8_t j = 0; j < BOARD_SIZE; j++){
				board[i][j]->piecePath[0] = false;
				board[i][j]->piecePath[1] = false;
				board[i][j]->pawnPath[0] = false;
				board[i][j]->pawnPath[1] = false;
			}
		}
		threatPath.clear();
	}
	for (int i = 0; i < 2; ++i) {
        KingLoc[i] = {-1,-1};
    }
    for (uint8_t i = 0; i < BOARD_SIZE; i++){
		for(uint8_t j = 0; j < BOARD_SIZE; j++){
			if(board[i][j]->chessChar != ' '){
				board[i][j]->checkSquares(i,j, threatPath);
				if(board[i][j]->chessChar == 'K') {
					bool colour = board[i][j]->pieceColour != BLUE;
					KingLoc[colour] = {(int8_t)i,(int8_t)j};
				}
			}
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
