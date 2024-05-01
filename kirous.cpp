#include "kirous.h"

chessUI::chessUI() {
	initscr();
	start_color();
	init_pair(RED_ON_WHITE, COLOR_RED, COLOR_WHITE); //Red player on white square
    init_pair(RED_ON_BLACK, COLOR_RED, COLOR_BLACK); //Red player on black square
	init_pair(BLUE_ON_WHITE, COLOR_BLUE, COLOR_WHITE); //Blue player on white square
    init_pair(BLUE_ON_BLACK, COLOR_BLUE, COLOR_BLACK); //Blue player on black square
    init_pair(TEXT_COLOUR, COLOR_BLACK, COLOR_CYAN); //Text colour / Blue attackable Squares
	#ifdef ThreatDebug
	init_pair(TEMP1, COLOR_YELLOW, COLOR_RED); // Red attackable squares
	init_pair(TEMP2, COLOR_YELLOW, COLOR_MAGENTA); //When both blue and red can attack on same square
	#endif
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
}

void chessUI::updateInterface(ChessBoard &cBoard, int ret) {
	clear();
	bool bg = 1;
	for (int i = 0; i < BOARD_SIZE ; i++){
		for (int j = 0; j < BOARD_SIZE; j++){
			char c = cBoard.getPieceChar(i,j);
			#ifdef ThreatDebug
			bool *threat = cBoard.getSquareThreat(i,j);
			#endif
			
			if(c != ' '){
				#ifdef ThreatDebug
				if(threat[0] && !threat[1]) attron(COLOR_PAIR(TEXT_COLOUR));
				else if(!threat[0] && threat[1])attron(COLOR_PAIR(TEMP1));
				else if(threat[0] && threat[1]) attron(COLOR_PAIR(TEMP2));
				else attron(COLOR_PAIR(bg+cBoard.getPieceColour(i,j)));
				#endif
				#ifndef ThreatDebug
				attron(COLOR_PAIR(bg+cBoard.getPieceColour(i,j)));
				#endif
				addch(c);
			}
			else {
				#ifdef ThreatDebug
				if(threat[0] && !threat[1]) attron(COLOR_PAIR(TEXT_COLOUR));
				else if(!threat[0] && threat[1])attron(COLOR_PAIR(TEMP1));
				else if(threat[0] && threat[1]) attron(COLOR_PAIR(TEMP2));
				else attron(COLOR_PAIR(bg+RED_ON_WHITE)); //Switches between black and white squares with bg
				#endif
				#ifndef ThreatDebug
				attron(COLOR_PAIR(bg+RED_ON_WHITE)); //Switches between black and white squares with bg
				#endif
				addch(' ');
			}
			bg = !bg;
		}
		bg = !bg;
		addch('\n');
	}
	attron(COLOR_PAIR(TEXT_COLOUR));
	if(!Init){getyx(stdscr,currentLoc.h,currentLoc.w);Init = true;}
	else {
	printw("Return code: %d", ret);
	}
	move(currentLoc.h, currentLoc.w);
	refresh();
}


int chessUI::Select(CursorLoc& loc){
	int a = false;
	while(!a){
		ch = getch();
		switch (ch) {
			case 10: //Enter key
				loc = currentLoc;
				a = true;
				break;
			case 'q':
				a = -6;
				break;
			case KEY_DOWN:
				currentLoc.h++;
				break;
			case KEY_UP:
				currentLoc.h--;
				break;
			case KEY_LEFT:
				currentLoc.w--;
				break;
			case KEY_RIGHT:
				currentLoc.w++;
				break;
		}
		move(currentLoc.h, currentLoc.w);
		refresh();
	}
	return a;
}

chessUI::~chessUI() {
	endwin();
}