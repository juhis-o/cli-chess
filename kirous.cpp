#include "kirous.h"

chessUI::chessUI() {
	initscr();
	start_color();
	init_pair(1, COLOR_RED, COLOR_WHITE);
    init_pair(2, COLOR_RED, COLOR_BLACK);
	init_pair(3, COLOR_BLUE, COLOR_WHITE);
    init_pair(4, COLOR_BLUE, COLOR_BLACK);
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
}

void chessUI::updateInterface(ChessBoard &cBoard) {
	clear();
	for (int i = 0; i < 8 ; i++){
		for (int j = 0; j < 8; j++){
			attron(COLOR_PAIR(cBoard.board[i][j].bgColour+cBoard.board[i][j].pieceColour));
			addch(cBoard.board[i][j].chessChar);
		}
		addch('\n');
	}
	move(8, 0);
	printw("coords: %d %d", currentLoc.h, currentLoc.w);
	if(!Init){getyx(stdscr,currentLoc.h,currentLoc.w);Init = true;}
	//refresh();
}

void chessUI::freeSelect(ChessBoard &cBoard){
	while((ch = getch()) != 'q'){
		switch (ch) {
			case KEY_LEFT:
				currentLoc.w--;
				break;
			case KEY_RIGHT:
				currentLoc.w++;
				break;
			case KEY_UP:
				currentLoc.h--;
				break;
			case KEY_DOWN:
				currentLoc.h++;
				break;
			case 10:
				enterPressed(cBoard);
			break;
		}
		move(currentLoc.h, currentLoc.w);
		refresh();
	}
}

void chessUI::enterPressed(ChessBoard &cBoard) {
	if(!selected){
		if(!(cBoard.board[currentLoc.h][currentLoc.w].chessChar == ' ')) {			
			selectedLoc = currentLoc;
			selected = !selected;
			}
		} 
		else {
			selected = !selected;
			cBoard.MovePiece(currentLoc,selectedLoc);
		}
	updateInterface(cBoard);
}

chessUI::~chessUI() {
	endwin();
}