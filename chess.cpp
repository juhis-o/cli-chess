#include "chess.h"

void chessPiece::moveEmptySpace(CursorLoc &newLoc, CursorLoc &oldLoc){
	board[newLoc.h][newLoc.w].swap(board[oldLoc.h][oldLoc.w]);
}

void chessPiece::moveOccupiedSpace(CursorLoc &newLoc, CursorLoc &oldLoc){
	board[newLoc.h][newLoc.w].swap(board[oldLoc.h][oldLoc.w]);
	board[oldLoc.h][oldLoc.w] = std::make_unique<emptyPiece>(board,0);
}

bool chessPiece::calcPath(CursorLoc &newLoc, CursorLoc &oldLoc){
    int8_t stepY = (oldLoc.h > newLoc.h) ? -1 : (oldLoc.h < newLoc.h) ? 1 : 0;
    int8_t stepX = (oldLoc.w > newLoc.w) ? -1 : (oldLoc.w < newLoc.w) ? 1 : 0;
    for(int8_t h = oldLoc.h + stepY, w = oldLoc.w + stepX; h != newLoc.h || w != newLoc.w; h += stepY, w += stepX){
        if (board[h][w]->chessChar != ' ') 
            return false;
    }
    return true;
}

void pawnPiece::checkSquares(int8_t h, int8_t w, std::vector<ThreatLoc>& loc){
	for(int8_t i = dirH, target = firstMove+abs(dirH)+1; abs(i) < target; i+=dirH){
		if(!inBounds(h+i,w))
			break;
		if((board[h+i][w]->chessChar == ' '))
			board[h+i][w]->pawnPath[colour] = true;
		else break;
	}
	
	for(int8_t i = -1; i < 2; i+=2){
		if(inBounds(h+dirH,w+i)){
			board[h+dirH][w+i]->piecePath[colour] = 2;
			if((board[h+dirH][w+i]->chessChar == 'K') && (pieceColour != board[h+dirH][w+i]->pieceColour)){
				ThreatLoc newThreat{h,w,false};
				loc.push_back(newThreat);
			}
		}
	}
}

chessPiece_retVals pawnPiece::forwardMove(CursorLoc &newLoc, CursorLoc &oldLoc, int8_t &movementY) {
	chessPiece_retVals ret = MOVE_OK;
	if(calcPath(newLoc,oldLoc) && board[newLoc.h][newLoc.w]->chessChar == ' ')
		if(abs(movementY) <= 1 + firstMove){
			moveEmptySpace(newLoc, oldLoc);
			if(firstMove && (abs(movementY) == 2))
				enPassantable = true;
			if(newLoc.h == promoteRank)
				ret = PAWN_PROMOTION;
			firstMove = false;
		}
		else
			ret = MOVE_NOT_VALID;
	else
		ret = PIECE_ON_PATH;
	
	return ret;
}

//				else if((board[i][j]->chessChar == 'S') && playerColours[!turn] == board[i][j]->pieceColour){
//					auto* pawn = dynamic_cast<pawnPiece*>(board[i][j].get());
//					if(pawn)pawn->enPassantable = false;
//				}

void pawnPiece::enPassantCapture(CursorLoc &newLoc, CursorLoc &oldLoc, int8_t h){
	board[newLoc.h][newLoc.w].swap(board[oldLoc.h][oldLoc.w]);
	board[oldLoc.h][oldLoc.w] = std::make_unique<emptyPiece>(board,0);
	board[h][newLoc.w] = std::make_unique<emptyPiece>(board,0);
}

chessPiece_retVals pawnPiece::captureMove(CursorLoc &newLoc, CursorLoc &oldLoc, int8_t &movementY) {
	chessPiece_retVals ret = MOVE_OK;
	if(board[newLoc.h][newLoc.w]->chessChar != ' ')
        if((pieceColour != board[newLoc.h][newLoc.w]->pieceColour)){
			moveOccupiedSpace(newLoc, oldLoc);
			if(newLoc.h == promoteRank)
				ret = PAWN_PROMOTION;
		}
		else
			ret = CAPTURING_OWN_PIECE;
	else if(oldLoc.h == enPassantRank) {
		int8_t enPass = (colour) ? newLoc.h-1 : newLoc.h+1;
		if(board[enPass][newLoc.w]->chessChar == 'S' && (pieceColour != board[enPass][newLoc.w]->pieceColour)){
			auto* pawn = dynamic_cast<pawnPiece*>(board[enPass][newLoc.w].get());
			if(pawn) {
				if(pawn->enPassantable) 
					enPassantCapture(newLoc,oldLoc,enPass);
				else
					ret = (chessPiece_retVals)enPass;
			}
		}	else
				ret = MOVE_NOT_VALID;
			
	}
	else 
		ret = PAWN_NO_PIECE_TO_CAPTURE;

	return ret;
}


chessPiece_retVals pawnPiece::move(CursorLoc &newLoc, CursorLoc &oldLoc){
	int8_t movementY = newLoc.h - oldLoc.h;
	int8_t movementX = newLoc.w - oldLoc.w;
	chessPiece_retVals ret = MOVE_OK;
	int8_t direction[2] = {0,0};
	direction[colour] = movementY;
	if(memcmp(&newLoc,&oldLoc,sizeof(newLoc)) == 0)
		return MOVE_CANCEL;
	if(direction[0] < direction[1])
		if(movementX == 0)
			ret = forwardMove(newLoc,oldLoc,movementY);
		else if(abs(movementX) == 1 && abs(movementY) == 1)
			ret = captureMove(newLoc,oldLoc,movementY);
		else
			ret = MOVE_NOT_VALID;
	else
		ret = PAWN_WRONG_DIR;

	return ret;
}

void towerPiece::checkSquares(int8_t h, int8_t w, std::vector<ThreatLoc>& loc){
	for(uint8_t d = 4; d < 8; d++) {
		int8_t i = directions[d][0], j = directions[d][1], dir1 = directions[d][0], dir2 = directions[d][1];
		bool piecePassed = false;
		for(;inBounds(h+i,w+j); i+=dir1,j+=dir2){
			if(!board[h+i][w+j]->piecePath[colour])
				board[h+i][w+j]->piecePath[colour] = true;
			if(piecePassed || board[h+i][w+j]->chessChar != ' '){
				if((board[h+i][w+j]->chessChar == 'K')){
					piecePassed = true;
					if((pieceColour != board[h+i][w+j]->pieceColour)) {
						ThreatLoc newThreat{h,w,false};
						loc.push_back(newThreat);
					}
				}
				else break;
			}
		}
	}
}

chessPiece_retVals towerPiece::move(CursorLoc &newLoc, CursorLoc &oldLoc){
	chessPiece_retVals ret = MOVE_OK;
	if(memcmp(&newLoc,&oldLoc,sizeof(newLoc)) == 0)
		return MOVE_CANCEL;
	if((newLoc.h == oldLoc.h) || (newLoc.w == oldLoc.w))
		if(calcPath(newLoc,oldLoc))
			if(board[newLoc.h][newLoc.w]->chessChar == ' ')
				moveEmptySpace(newLoc, oldLoc);
			else
            	if((pieceColour != board[newLoc.h][newLoc.w]->pieceColour))
					moveOccupiedSpace(newLoc, oldLoc);
				else
					ret = CAPTURING_OWN_PIECE;
		else
			ret = PIECE_ON_PATH;
	else
		ret = MOVE_NOT_VALID;
	return ret;
}

void bishopPiece::checkSquares(int8_t h, int8_t w, std::vector<ThreatLoc>& loc){
	for(uint8_t d = 0; d < 4; d++) {
		bool piecePassed = false;
		int8_t i = directions[d][0], j = directions[d][1], dir1 = directions[d][0], dir2 = directions[d][1];
		for(;inBounds(h+i,w+j); i+=dir1, j+=dir2) {
			if(!board[h+i][w+j]->piecePath[colour])
				board[h+i][w+j]->piecePath[colour] = true;
			if(piecePassed || board[h+i][w+j]->chessChar != ' ') {
				if((board[h+i][w+j]->chessChar == 'K')) {
					piecePassed = true;
					if((pieceColour != board[h+i][w+j]->pieceColour)){
						ThreatLoc newThreat{h,w,false};
						loc.push_back(newThreat);
					}
				}
				else break;
			}
		}
	}
}

chessPiece_retVals bishopPiece::move(CursorLoc &newLoc, CursorLoc &oldLoc){
	int8_t movementY = newLoc.h - oldLoc.h;
	int8_t movementX = newLoc.w - oldLoc.w;
	chessPiece_retVals ret = MOVE_OK;
	if(memcmp(&newLoc,&oldLoc,sizeof(newLoc)) == 0)
		return MOVE_CANCEL;
	if(abs(movementX) == abs(movementY)) 
		if(calcPath(newLoc,oldLoc)) 
			if(board[newLoc.h][newLoc.w]->chessChar == ' ')
				moveEmptySpace(newLoc, oldLoc);
			else 
            	if((pieceColour != board[newLoc.h][newLoc.w]->pieceColour))
					moveOccupiedSpace(newLoc, oldLoc);
				else
					ret = CAPTURING_OWN_PIECE;
		else
			ret = PIECE_ON_PATH;
	else
		ret = MOVE_NOT_VALID;

	return ret;
}

void horsePiece::checkSquares(int8_t h, int8_t w, std::vector<ThreatLoc>& loc){
	bool f = false;
	int8_t hMov[2] = {1,2};
	for(int8_t i = 0; i < 8; i++, !(i%2) ? hMov[0]*=-1 : hMov[1]*=-1, f = !(i%4) ? !f : f) {
		if(!inBounds(h+hMov[f],w+hMov[!f]))
			continue;
		if(!board[h+hMov[f]][w+hMov[!f]]->piecePath[colour])
			board[h+hMov[f]][w+hMov[!f]]->piecePath[colour] = true;
		if((board[h+hMov[f]][w+hMov[!f]]->chessChar == 'K'))
			if((pieceColour != board[h+hMov[f]][w+hMov[!f]]->pieceColour)){
				ThreatLoc newThreat{h,w,true};
				loc.push_back(newThreat);
			}
	}
}

chessPiece_retVals horsePiece::move(CursorLoc &newLoc, CursorLoc &oldLoc){
	int8_t movementY = newLoc.h - oldLoc.h;
	int8_t movementX = newLoc.w - oldLoc.w;
	chessPiece_retVals ret = MOVE_OK;
	if(memcmp(&newLoc,&oldLoc,sizeof(newLoc)) == 0)
		return MOVE_CANCEL;
	if((abs(movementX) == 1 && abs(movementY) == 2) || (abs(movementX) == 2 && abs(movementY) == 1))
		if(board[newLoc.h][newLoc.w]->chessChar == ' ')
			moveEmptySpace(newLoc, oldLoc);
		else
            if(pieceColour != board[newLoc.h][newLoc.w]->pieceColour)
				moveOccupiedSpace(newLoc, oldLoc);
			else
				ret = CAPTURING_OWN_PIECE;
	else
		ret = MOVE_NOT_VALID;

	return ret;
}


void queenPiece::checkSquares(int8_t h, int8_t w, std::vector<ThreatLoc>& loc){
	for(uint8_t d = 0; d < 8; d++) {
		int8_t i = directions[d][0], j = directions[d][1], dir1 = directions[d][0], dir2 = directions[d][1];
		bool piecePassed = false;
		for(;inBounds(h+i,w+j); i+=dir1, j+=dir2) {
			if(!board[h+i][w+j]->piecePath[colour])
				board[h+i][w+j]->piecePath[colour] = true;
			if(piecePassed || board[h+i][w+j]->chessChar != ' '){
				if((board[h+i][w+j]->chessChar == 'K')){
					if((pieceColour != board[h+i][w+j]->pieceColour)){
						ThreatLoc newThreat{h,w,false};
						loc.push_back(newThreat);
					}
					piecePassed = true;
				}
				else break;
			}
		}
	}
}

chessPiece_retVals queenPiece::move(CursorLoc &newLoc, CursorLoc &oldLoc){
	int8_t movementY = newLoc.h - oldLoc.h;
	int8_t movementX = newLoc.w - oldLoc.w;
	chessPiece_retVals ret = MOVE_OK;
	if(memcmp(&newLoc,&oldLoc,sizeof(newLoc)) == 0)
		return MOVE_CANCEL;
	if((abs(movementX) == abs(movementY)) || ((newLoc.h == oldLoc.h) || (newLoc.w == oldLoc.w)))
		if(calcPath(newLoc,oldLoc))
			if(board[newLoc.h][newLoc.w]->chessChar == ' ')
				moveEmptySpace(newLoc, oldLoc);
			else 
            	if (pieceColour != board[newLoc.h][newLoc.w]->pieceColour)
					moveOccupiedSpace(newLoc, oldLoc);
				else
					ret = CAPTURING_OWN_PIECE;
		else
			ret = PIECE_ON_PATH;
	else
		ret = MOVE_NOT_VALID;

	return ret;
}

void kingPiece::checkSquares(int8_t h, int8_t w, std::vector<ThreatLoc>& loc){
	(void)loc;
	for(int8_t d = 0; d < 8; d++) {
		if(!inBounds(h+directions[d][0],w+directions[d][1]))
			continue;
		if(!board[h+directions[d][0]][w+directions[d][1]]->piecePath[colour]) 
			board[h+directions[d][0]][w+directions[d][1]]->piecePath[colour] = 3; //Kings threat squares
	}
}

chessPiece_retVals kingPiece::move(CursorLoc &newLoc, CursorLoc &oldLoc){
	int8_t movementY = newLoc.h - oldLoc.h;
	int8_t movementX = newLoc.w - oldLoc.w;
	chessPiece_retVals ret = MOVE_OK;
	if(memcmp(&newLoc,&oldLoc,sizeof(newLoc)) == 0)
		return MOVE_CANCEL;
	if(abs(movementX) <= 1 && abs(movementY) <= 1)
		if(board[newLoc.h][newLoc.w]->chessChar == ' ')
			if(!board[newLoc.h][newLoc.w]->piecePath[!colour])
				moveEmptySpace(newLoc, oldLoc);
			else
				ret = THREAT;
		else
            if(pieceColour != board[newLoc.h][newLoc.w]->pieceColour)
				if(!board[newLoc.h][newLoc.w]->piecePath[!colour])
					moveOccupiedSpace(newLoc, oldLoc);
				else
					ret = THREAT;
			else
				ret = CAPTURING_OWN_PIECE;
	else
		ret = MOVE_NOT_VALID;

	return ret;
}

enum CHECKMATE_STATE ChessBoard::checkmate(bool playerTurn){
	bool colour = board[KingLoc[playerTurn].h][KingLoc[playerTurn].w]->pieceColour != BLUE;
	CHECKMATE_STATE ret = KING_SAFE;
	if(!board[KingLoc[playerTurn].h][KingLoc[playerTurn].w]->piecePath[!colour])
		return ret; //No piece is threatening king
	else
		ret = KING_THREATENED;
	if(canKingMove(KingLoc[playerTurn]))
		return ret; //King can move out of threat square
	for (auto& x : threatPath) {
		if(findOverlaps(KingLoc[playerTurn], x))
			return ret; //Threatening piece can be captured or other piece can move in its way
	}

	return CHECKMATE;
}


bool ChessBoard::canKingMove(CursorLoc& KingLoc){
	bool colour = board[KingLoc.h][KingLoc.w]->pieceColour != BLUE;
	bool canMove = false;
	for(int8_t i = 0; i < 8; i++ ) {
		if(!inBounds(KingLoc.h+directions[i][0],KingLoc.w+directions[i][1]) ||
		(board[KingLoc.h][KingLoc.w]->pieceColour == board[KingLoc.h+directions[i][0]][KingLoc.w+directions[i][1]]->pieceColour)) //Skip iteration, if square is out of bounds or if piece is same color as king
			continue;
		if(!board[KingLoc.h+directions[i][0]][KingLoc.w+directions[i][1]]->piecePath[!colour]){
			canMove = true;
			break;
		}
	}

	return canMove;
}

bool ChessBoard::findOverlaps(CursorLoc &kingLoc, ThreatLoc &threatPiece){
    int8_t stepY = (threatPiece.h > kingLoc.h) ? -1 : (threatPiece.h < kingLoc.h) ? 1 : 0;
    int8_t stepX = (threatPiece.w > kingLoc.w) ? -1 : (threatPiece.w < kingLoc.w) ? 1 : 0;
    bool colour = board[kingLoc.h][kingLoc.w]->pieceColour != BLUE;
    if(!threatPiece.horsePiece){
		if(board[threatPiece.h][threatPiece.w]->piecePath[colour])
			return true;
        for(int8_t h = threatPiece.h, w = threatPiece.w; h != kingLoc.h || w != kingLoc.w; h += stepY, w += stepX){
            if(h < 0 || h >= BOARD_SIZE || w < 0 || w >= BOARD_SIZE)
				break;
            if((board[h][w]->piecePath[0] == 1 && board[h][w]->piecePath[1] == 1) || (board[h][w]->pawnPath[colour]))
                return true;
        }
    } 
	else {
        if((board[threatPiece.h][threatPiece.w]->piecePath[!colour]) ||
		(board[threatPiece.h][threatPiece.w]->piecePath[colour] && board[threatPiece.h][threatPiece.w]->pawnPath[!colour]))
			return true;
    }

    return false;
}

ChessBoard::ChessBoard() {
	uint8_t unit_colour = 1;
	KingLoc[0] = KingLoc[1] = {0,0};
	board.resize(BOARD_SIZE);
    for(uint8_t i = 0; i < BOARD_SIZE; i++){
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
		if(i == 1)
			unit_colour+=2;
	}
	updateThreatSquares(false);
}

chessPiece_retVals ChessBoard::setPiece(CursorLoc loc, char piece, bool color){
	uint8_t pColour = (color) ? BLUE : RED;
	switch(piece) {
		case 'B':
			board[loc.h][loc.w] = std::make_unique<bishopPiece>(board,pColour);
			break;
		case 'H':
			board[loc.h][loc.w] = std::make_unique<horsePiece>(board,pColour);
			break;
		case 'Q':
			board[loc.h][loc.w] = std::make_unique<queenPiece>(board,pColour);
			break;
		case 'T':
			board[loc.h][loc.w] = std::make_unique<towerPiece>(board,pColour);
			break;
	}
	updateThreatSquares(true);

	return PAWN_PROMOTED;
}

void ChessBoard::FillRow(uint8_t row, uint8_t& unit_colour, std::vector<std::unique_ptr<chessPiece>>&Board){
	switch(row) {
		case BACKROW:
			for(uint8_t i = 0; i < BOARD_SIZE; i++) {
				switch(i) {
					case 0: case 7:
						Board[i] = std::make_unique<towerPiece>(board,unit_colour);
						break;
					case 1: case 6:
						Board[i] = std::make_unique<horsePiece>(board,unit_colour);
						break;
					case 2: case 5:
						Board[i] = std::make_unique<bishopPiece>(board,unit_colour);
						break;
					case 3:
						Board[i] = std::make_unique<queenPiece>(board,unit_colour);
						break;
					case 4:
						Board[i] = std::make_unique<kingPiece>(board,unit_colour);
						break;
					}
			}
			break;
		case FRONTROW:
			for(uint8_t i = 0; i < BOARD_SIZE; i++){
				Board[i] = std::make_unique<pawnPiece>(board,unit_colour);
			}
			break;
		default:
			for(uint8_t i = 0; i < BOARD_SIZE; i++){
				Board[i] = std::make_unique<emptyPiece>(board,0);
			}
			break;
	}
}

void ChessBoard::updateThreatSquares(bool reset) {
	if(reset) {
		for (int8_t i = 0; i < BOARD_SIZE; i++){
			for(int8_t j = 0; j < BOARD_SIZE; j++){
				board[i][j]->piecePath[0] = false;
				board[i][j]->piecePath[1] = false;
				board[i][j]->pawnPath[0] = false;
				board[i][j]->pawnPath[1] = false;
			}
		}
		threatPath.clear();
	}
    for(int8_t i = 0; i < BOARD_SIZE; i++){
		for(int8_t j = 0; j < BOARD_SIZE; j++){
			if(board[i][j]->chessChar != ' '){
				board[i][j]->checkSquares(i,j, threatPath);
				if(board[i][j]->chessChar == 'K') {
					bool colour = board[i][j]->pieceColour != BLUE;
					KingLoc[colour] = {i,j};
				}
			}
		}
	}

}

void ChessBoard::updateThreatSquares(bool reset, bool turn) {
	if(reset) {
		for (int8_t i = 0; i < BOARD_SIZE; i++){
			for(int8_t j = 0; j < BOARD_SIZE; j++){
				board[i][j]->piecePath[0] = false;
				board[i][j]->piecePath[1] = false;
				board[i][j]->pawnPath[0] = false;
				board[i][j]->pawnPath[1] = false;
			}
		}
		threatPath.clear();
	}
    for(int8_t i = 0; i < BOARD_SIZE; i++){
		for(int8_t j = 0; j < BOARD_SIZE; j++){
			if(board[i][j]->chessChar != ' '){
				board[i][j]->checkSquares(i,j, threatPath);
				if(board[i][j]->chessChar == 'K') {
					bool colour = board[i][j]->pieceColour != BLUE;
					KingLoc[colour] = {i,j};
				}
				else if((board[i][j]->chessChar == 'S') && playerColours[!turn] == board[i][j]->pieceColour){
					auto* pawn = dynamic_cast<pawnPiece*>(board[i][j].get());
					if(pawn)pawn->enPassantable = false;
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
