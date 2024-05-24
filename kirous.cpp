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
	init_pair(TEMP2, COLOR_YELLOW, COLOR_MAGENTA); // When both blue and red can attack on same square
	init_pair(TEMP3, COLOR_BLACK, COLOR_YELLOW); // Red attackable squares
	init_pair(TEMP4, COLOR_YELLOW, COLOR_WHITE); // When both blue and red can attack on same square
	#endif
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
}

void chessUI::updateInterface(ChessBoard &cBoard, int ret, int8_t checkmate) {
	clear();
	bool bg = 1;
	for (int i = 0; i < BOARD_SIZE ; i++){
		for (int j = 0; j < BOARD_SIZE; j++){
			char c = cBoard.getPieceChar(i,j);
			#ifdef ThreatDebug
			uint8_t *threat = cBoard.getSquareThreat(i,j);
//			bool *threat = cBoard.getPawnPath(i,j);
//			bool kThreat = cBoard.getKingThreat(i,j);
			#endif
			
			if(c != ' '){
				#ifdef ThreatDebug
				if(threat[0] == 1 && threat[1] == 0) attron(COLOR_PAIR(TEXT_COLOUR));
				else if(threat[0] == 0 && threat[1] == 1)attron(COLOR_PAIR(TEMP1));
				else if(threat[0] == 1 && threat[1] == 1) attron(COLOR_PAIR(TEMP2));
				else if(threat[0] == 2 || threat[1] == 2) attron(COLOR_PAIR(TEMP3));
				//if(kThreat) attron(COLOR_PAIR(TEXT_COLOUR));
				else attron(COLOR_PAIR(bg+cBoard.getPieceColour(i,j)));
				#endif
				#ifndef ThreatDebug
				attron(COLOR_PAIR(bg+cBoard.getPieceColour(i,j)));
				#endif
				addch(c);
			}
			else {
				#ifdef ThreatDebug
				if(threat[0] == 1 && threat[1] == 0) attron(COLOR_PAIR(TEXT_COLOUR));
				else if(threat[0] == 0 && threat[1] == 1)attron(COLOR_PAIR(TEMP1));
				else if(threat[0] == 1 && threat[1] == 1) attron(COLOR_PAIR(TEMP2));
				else if(threat[0] == 2 || threat[1] == 2) attron(COLOR_PAIR(TEMP3));
				//if(kThreat) attron(COLOR_PAIR(TEXT_COLOUR));
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
	if(checkmate == 1) {
		printw("Checkmate! Press Q to quit");
		gameover = true;
	}
	else if(checkmate == -1) {
		printw("King is being threatened! Find way to stop threat");
	}
	else if(checkmate == -2) {
		printw("Player has failed to protect the king");
		gameover = true;
	}
//	printw("Return code: %d; %d", ret, checkmate);
	}
	move(currentLoc.h, currentLoc.w);
	refresh();
}


int chessUI::Select(CursorLoc& loc){
	int ret = false;
	while(!ret){
		ch = getch();
		switch (ch) {
			case 10: //Enter key
				if(!gameover) {
					loc = currentLoc;
					ret = true;
				}
				break;
			case 'q':
				ret = -6;
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
	return ret;
}

chessUI::~chessUI() {
	endwin();
}