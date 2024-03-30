#include "chess.h"

ChessBoard::ChessBoard() {
	bool background_colour = 1;
	int unit_colour = 1;
	char backrow[8] = {'T','H','B','K','Q','B','H','T'};
    char frontrow[8] = {'S','S','S','S','S','S','S','S'};
    char emptyrow[8] = {' ',' ',' ',' ',' ',' ',' ',' '};
    for (int i = 0; i < BOARD_SIZE; i++){
		switch(i) {
			case 0: case 7:
				FillRow(backrow, background_colour,unit_colour, board[i]);
				break;
			case 1: case 6:
				FillRow(frontrow, background_colour,unit_colour, board[i]);
				break;
			default:
				FillRow(emptyrow, background_colour,unit_colour, board[i]);
				break;
		}
		if(i == 1) unit_colour+=2;
	    }
}

void ChessBoard::MovePiece(CursorLoc &newLoc, CursorLoc &oldLoc){
	board[newLoc.h][newLoc.w].chessChar = board[oldLoc.h][oldLoc.w].chessChar;
	board[newLoc.h][newLoc.w].pieceColour = board[oldLoc.h][oldLoc.w].pieceColour;
	board[oldLoc.h][oldLoc.w].chessChar= ' ';
}

void ChessBoard::FillRow(char* row, bool& bgColour, int& unit_colour, chessPiece* Board) {
	for(int i = 0; i < 8; i++){
		bgColour = !bgColour;
		Board[i].bgColour = bgColour;
		Board[i].pieceColour = unit_colour;
		Board[i].chessChar = row[i];
	}
    bgColour = !bgColour;

}
