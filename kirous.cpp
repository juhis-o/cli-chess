#include "kirous.h"

chessUI::chessUI() {
	initscr();
	start_color();
	init_pair(RED_ON_WHITE, COLOR_RED, COLOR_WHITE); //Red player on white square
    init_pair(RED_ON_BLACK, COLOR_RED, COLOR_BLACK); //Red player on black square
	init_pair(BLUE_ON_WHITE, COLOR_BLUE, COLOR_WHITE); //Blue player on white square
    init_pair(BLUE_ON_BLACK, COLOR_BLUE, COLOR_BLACK); //Blue player on black square
    init_pair(TEXT_COLOUR, COLOR_BLACK, COLOR_CYAN); //Text colour
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
}

void chessUI::updateInterface(ChessBoard &cBoard) {
	clear();
	for (int i = 0; i < 8 ; i++){
		for (int j = 0; j < 8; j++){
			attron(COLOR_PAIR(cBoard.board[i][j]->bgColour+cBoard.board[i][j]->pieceColour));
			addch(cBoard.board[i][j]->chessChar);
		}
		addch('\n');
	}
	//move(8, 0);
	attron(COLOR_PAIR(TEXT_COLOUR));
	if(!Init){getyx(stdscr,currentLoc.h,currentLoc.w);Init = true;}
	else {
	printw("Return code: %d %d", ret, selected);
	}
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
	if(!selected) {
		if (currentLoc.h >= 0 && currentLoc.h < cBoard.board.size() &&
            currentLoc.w >= 0 && currentLoc.w < cBoard.board[0].size()) {
			if(!(cBoard.board[currentLoc.h][currentLoc.w]->isEmpty) && ((playerTurn && cBoard.board[currentLoc.h][currentLoc.w]->pieceColour == BLUE) || (!playerTurn && cBoard.board[currentLoc.h][currentLoc.w]->pieceColour == RED))) {
				selectedLoc = currentLoc;
				selected = true;
			}
		}
		else ret = -100;
	}
	else {
		if (currentLoc.h >= 0 && currentLoc.h < cBoard.board.size() &&
            currentLoc.w >= 0 && currentLoc.w < cBoard.board[0].size()) {
				ret = cBoard.board[selectedLoc.h][selectedLoc.w]->move(currentLoc, selectedLoc, cBoard.board);
				if(ret > 0) {
					playerTurn = !playerTurn;
				}
				selected = false;
			}
		else ret = -100;
	}
	updateInterface(cBoard);
}

chessUI::~chessUI() {
	endwin();
}