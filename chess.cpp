#include "chess.h"

void chessPiece::moveEmptySpace(CursorLoc &newLoc, CursorLoc &oldLoc){
	board[newLoc.h][newLoc.w].swap(board[oldLoc.h][oldLoc.w]);
}

void chessPiece::moveOccupiedSpace(CursorLoc &newLoc, CursorLoc &oldLoc){
	board[newLoc.h][newLoc.w].swap(board[oldLoc.h][oldLoc.w]);
	board[oldLoc.h][oldLoc.w] = std::make_unique<emptyPiece>(board);
}

bool chessPiece::calcPath(CursorLoc &newLoc, CursorLoc &oldLoc){
    int8_t stepY = (oldLoc.h > newLoc.h) ? -1 : (oldLoc.h < newLoc.h) ? 1 : 0;
    int8_t stepX = (oldLoc.w > newLoc.w) ? -1 : (oldLoc.w < newLoc.w) ? 1 : 0;
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
		if(!inBounds(h+i,w)) break;
		if((board[h+i][w]->chessChar == ' ')) board[h+i][w]->pawnPath[colour] = true;
		else break;
	}

	for(int8_t i = -1; i < 2; i+=2) {
		if(inBounds(h+dirH,w+i)){
			board[h+dirH][w+i]->piecePath[colour] = 2;
			if((board[h+dirH][w+i]->chessChar == 'K') && (playerColours[colour] != board[h+dirH][w+i]->pieceColour)) {
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
	bool colour = pieceColour != BLUE;
	int8_t direction[2] = {0,0};
	direction[colour] = movementY;

	if(memcmp(&newLoc,&oldLoc,sizeof(newLoc)) == 0) return MOVE_CANCEL;
	if(direction[0] < direction[1]){
		if(movementX == 0) {
			if(calcPath(newLoc,oldLoc) && board[newLoc.h][newLoc.w]->chessChar == ' '){
				if(abs(movementY) <= 1 + firstMove) {
					firstMove = false;
					moveEmptySpace(newLoc, oldLoc);
					if(promotionH[colour] == newLoc.h) ret = PAWN_PROMOTION;
				}
				else ret = MOVE_NOT_VALID;
			}
			else ret = PIECE_ON_PATH;
		}
		else if((abs(movementX) == 1 && abs(movementY) == 1) && 
		(board[newLoc.h][newLoc.w]->chessChar != ' ')){
        	if((playerColours[colour] != board[newLoc.h][newLoc.w]->pieceColour)){
				moveOccupiedSpace(newLoc, oldLoc);
				if(promotionH[colour] == newLoc.h) ret = PAWN_PROMOTION;
			}
			else ret = CAPTURING_OWN_PIECE;
		}
		else ret = MOVE_NOT_VALID;
	}
	else ret = PAWN_WRONG_DIR;

	return ret;
}

void towerPiece::checkSquares(int h, int w, std::vector<ThreatLoc>& loc){
	bool colour = pieceColour != BLUE;
	for(uint8_t d = 4; d < 8; d++) {
		int8_t i = directions[d][0], j = directions[d][1], dir1 = directions[d][0], dir2 = directions[d][1];
		bool piecePassed = false;
		for(;inBounds(h+i,w+j); i+=dir1,j+=dir2){
			if(!board[h+i][w+j]->piecePath[colour])board[h+i][w+j]->piecePath[colour] = true;
			if(piecePassed || board[h+i][w+j]->chessChar != ' ') {
				if((board[h+i][w+j]->chessChar == 'K')) {
					piecePassed = true;
					if((playerColours[colour] != board[h+i][w+j]->pieceColour)) {
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
	
	if(memcmp(&newLoc,&oldLoc,sizeof(newLoc)) == 0) return MOVE_CANCEL;
	if((newLoc.h == oldLoc.h) || (newLoc.w == oldLoc.w)){
		if(calcPath(newLoc,oldLoc)) {
			if(board[newLoc.h][newLoc.w]->chessChar == ' '){
				moveEmptySpace(newLoc, oldLoc);
			}
			else {
            	if((playerColours[colour] != board[newLoc.h][newLoc.w]->pieceColour)){
					moveOccupiedSpace(newLoc, oldLoc);
				}
				else ret = CAPTURING_OWN_PIECE;
			}
		}
		else ret = PIECE_ON_PATH;
	}
	else ret = MOVE_NOT_VALID;
	return ret;
}

void bishopPiece::checkSquares(int h, int w, std::vector<ThreatLoc>& loc){
	bool colour = pieceColour != BLUE;
	for(uint8_t d = 0; d < 4; d++) {
		bool piecePassed = false;
		int8_t i = directions[d][0], j = directions[d][1], dir1 = directions[d][0], dir2 = directions[d][1];
		for(;inBounds(h+i,w+j); i+=dir1, j+=dir2) {
			if(!board[h+i][w+j]->piecePath[colour]) board[h+i][w+j]->piecePath[colour] = true;
			if(piecePassed || board[h+i][w+j]->chessChar != ' ') {
				if((board[h+i][w+j]->chessChar == 'K')) {
					piecePassed = true;
					if((playerColours[colour] != board[h+i][w+j]->pieceColour)){
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

	if(memcmp(&newLoc,&oldLoc,sizeof(newLoc)) == 0) return MOVE_CANCEL;
	if(abs(movementX) == abs(movementY)) {
		if(calcPath(newLoc,oldLoc)) {
			if(board[newLoc.h][newLoc.w]->chessChar == ' '){
				moveEmptySpace(newLoc, oldLoc);
			}
			else {
            	if((playerColours[colour] != board[newLoc.h][newLoc.w]->pieceColour)) {
					moveOccupiedSpace(newLoc, oldLoc);
				}
				else ret = CAPTURING_OWN_PIECE;
			}
		}
		else ret = PIECE_ON_PATH;
	}
	else ret = MOVE_NOT_VALID;

	return ret;
}

void horsePiece::checkSquares(int h, int w, std::vector<ThreatLoc>& loc){
	bool colour = pieceColour != BLUE;
	int8_t hMov[2] = {1,2};
	bool f = false;
	for(int i = 0; i < 8; i++, !(i%2) ? hMov[0]*=-1 : hMov[1]*=-1, f = !(i%4) ? !f : f) {
		if(!inBounds(h+hMov[f],w+hMov[!f])) continue;
		if(!board[h+hMov[f]][w+hMov[!f]]->piecePath[colour])board[h+hMov[f]][w+hMov[!f]]->piecePath[colour] = true;
		if((board[h+hMov[f]][w+hMov[!f]]->chessChar == 'K')) {
			if((playerColours[colour] != board[h+hMov[f]][w+hMov[!f]]->pieceColour)){
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

	if(memcmp(&newLoc,&oldLoc,sizeof(newLoc)) == 0) return MOVE_CANCEL;
	if((abs(movementX) == 1 && abs(movementY) == 2) || (abs(movementX) == 2 && abs(movementY) == 1)){
		if(board[newLoc.h][newLoc.w]->chessChar == ' '){
			moveEmptySpace(newLoc, oldLoc);
		}
		else {
            if(playerColours[colour] != board[newLoc.h][newLoc.w]->pieceColour){
				moveOccupiedSpace(newLoc, oldLoc);
			}
			else ret = CAPTURING_OWN_PIECE;
		}
	}
	else ret = MOVE_NOT_VALID;
	return ret;
}


void queenPiece::checkSquares(int h, int w, std::vector<ThreatLoc>& loc){
	bool colour = pieceColour != BLUE;
	for(uint8_t d = 0; d < 8; d++) {
		int8_t i = directions[d][0], j = directions[d][1], dir1 = directions[d][0], dir2 = directions[d][1];
		bool piecePassed = false;
		for(;inBounds(h+i,w+j); i+=dir1, j+=dir2) {
			if(!board[h+i][w+j]->piecePath[colour]) board[h+i][w+j]->piecePath[colour] = true;
			if(piecePassed || board[h+i][w+j]->chessChar != ' ') {
				if((board[h+i][w+j]->chessChar == 'K')) {
					if((playerColours[colour] != board[h+i][w+j]->pieceColour)) {
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

	if(memcmp(&newLoc,&oldLoc,sizeof(newLoc)) == 0) return MOVE_CANCEL;
	if((abs(movementX) == abs(movementY)) || ((newLoc.h == oldLoc.h) || (newLoc.w == oldLoc.w))) {
		if(calcPath(newLoc,oldLoc)) {
			if(board[newLoc.h][newLoc.w]->chessChar == ' '){
				moveEmptySpace(newLoc, oldLoc);
			}
			else {
            	if (playerColours[colour] != board[newLoc.h][newLoc.w]->pieceColour) {
					moveOccupiedSpace(newLoc, oldLoc);
				}
				else ret = CAPTURING_OWN_PIECE;
			}
		}
		else ret = PIECE_ON_PATH;
	}
	else ret = MOVE_NOT_VALID;

	return ret;
}

void kingPiece::checkSquares(int h, int w, std::vector<ThreatLoc>& loc){
	(void)loc;
	bool colour = pieceColour != BLUE;
	for(int i = 0; i < 8; i++) {
		if(!inBounds(h+directions[i][0],w+directions[i][1])) continue;
		if(!board[h+directions[i][0]][w+directions[i][1]]->piecePath[colour])board[h+directions[i][0]][w+directions[i][1]]->piecePath[colour] = 3; //Kings threat squares
	}
}

int kingPiece::move(CursorLoc &newLoc, CursorLoc &oldLoc){
	int8_t movementY = newLoc.h - oldLoc.h;
	int8_t movementX = newLoc.w - oldLoc.w;
	bool colour = pieceColour != BLUE;
	int ret = MOVE_OK;

	if(memcmp(&newLoc,&oldLoc,sizeof(newLoc)) == 0) return MOVE_CANCEL;
	if(abs(movementX) <= 1 && abs(movementY) <= 1) {
		if(board[newLoc.h][newLoc.w]->chessChar == ' '){
			if(!board[newLoc.h][newLoc.w]->piecePath[!colour]) moveEmptySpace(newLoc, oldLoc);
			else ret = THREAT;
		}
		else {
            if (playerColours[colour] != board[newLoc.h][newLoc.w]->pieceColour) {
				if(!board[newLoc.h][newLoc.w]->piecePath[!colour]) moveOccupiedSpace(newLoc, oldLoc);
				else ret = THREAT;
			}
			else ret = CAPTURING_OWN_PIECE;
		}
	}
	else ret = MOVE_NOT_VALID;

	return ret;
}

enum CHECKMATE_STATE ChessBoard::checkmate(bool playerTurn){
	bool colour = board[KingLoc[playerTurn].h][KingLoc[playerTurn].w]->pieceColour != BLUE;
	CHECKMATE_STATE ret = KING_SAFE;
	if(!board[KingLoc[playerTurn].h][KingLoc[playerTurn].w]->piecePath[!colour]) return ret; //No piece is threatening king
	else ret = KING_THREATENED;
	if(canKingMove(KingLoc[playerTurn])) return ret; //King can move out of threat square
	for (auto& x : threatPath) {
		if(findOverlaps(KingLoc[playerTurn], x)) return ret; //Threatening piece can be captured or other piece can move in its way
	}
	return CHECKMATE;
}


bool ChessBoard::canKingMove(CursorLoc& KingLoc){
	bool colour = board[KingLoc.h][KingLoc.w]->pieceColour != BLUE;
	bool canMove = false;
	for(int i = 0; i < 8; i++ ) {
		if(!inBounds(KingLoc.h+directions[i][0],KingLoc.w+directions[i][1]) ||
		(board[KingLoc.h+directions[i][0]][KingLoc.w+directions[i][1]]->pieceColour == board[KingLoc.h][KingLoc.w]->pieceColour)) continue;
		if(!board[KingLoc.h+directions[i][0]][KingLoc.w+directions[i][1]]->piecePath[!colour]){
			canMove = true;
			break;
		}
	}
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

int ChessBoard::setPiece(CursorLoc loc, char piece, bool color){
	uint8_t pColour = (color) ? BLUE : RED;
	switch(piece) {
		case 'B':
			board[loc.h][loc.w] = std::make_unique<bishopPiece>(board);
			break;
		case 'H':
			board[loc.h][loc.w] = std::make_unique<horsePiece>(board);
			break;
		case 'Q':
			board[loc.h][loc.w] = std::make_unique<queenPiece>(board);
			break;
		case 'T':
			board[loc.h][loc.w] = std::make_unique<towerPiece>(board);
			break;
	}
	board[loc.h][loc.w]->pieceColour = pColour;
	updateThreatSquares(true);
	return PAWN_PROMOTED;
}

void ChessBoard::FillRow(int row, uint8_t& unit_colour, std::vector<std::unique_ptr<chessPiece>>&Board) {
	switch(row) {
		case BACKROW:
			for(int i = 0; i < BOARD_SIZE; i++) {
				switch(i) {
					case 0: case 7:
						Board[i] = std::make_unique<towerPiece>(board);
						break;
					case 1: case 6:
						Board[i] = std::make_unique<horsePiece>(board);
						break;
					case 2: case 5:
						Board[i] = std::make_unique<bishopPiece>(board);
						break;
					case 3:
						Board[i] = std::make_unique<queenPiece>(board);
						break;
					case 4:
						Board[i] = std::make_unique<kingPiece>(board);
						break;
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
			Board[3] = std::make_unique<towerPiece>(board);
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
}
*/
