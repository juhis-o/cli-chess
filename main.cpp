#include <iostream>
#include <ncurses.h>
#include <cstring>

char board[8][8];

char backrow[8] = {'T','H','B','K','Q','B','H','T'};
char frontrow[8] = {'S','S','S','S','S','S','S','S'};
char emptyrow[8] = {'#','#','#','#','#','#','#','#'};


void FillRow(char* row, char* boardRow) {
	for (int j = 0; j < 8; j++) {
		boardRow[j] = row[j];
	}
}



int main(){
	//std::cout << "Hello world!" << std::endl;

	bool char_selected = false;
	//int letters = 33;
	int select = 0;
	int ch = 0;
	//memcpy(board[0],backrow,sizeof(backrow));
	for (int i = 0; i < 8 ; i++){
		switch(i) {
			case 0: case 7:
				FillRow(backrow,board[i]);
				break;
			case 1: case 6:
				FillRow(frontrow,board[i]);
				break;
			default:
				FillRow(emptyrow,board[i]);
				break;
		}
	}

	//printf("%s",board);


    initscr();
	start_color();
	init_pair(1, COLOR_BLACK, COLOR_WHITE);
    init_pair(2, COLOR_WHITE, COLOR_BLACK);
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

   	//getmaxyx(stdscr, h, w);
	//std::cout << h << "H, " << w << "W " << std::endl;
    // Print a message
  

    // Refresh the screen to display the message
    
	

	for (int i = 0; i < 8 ; i++){
		for (int j = 0; j < 8 ; j++) {
			if (i == 0 || i == 1) attron(COLOR_PAIR(1));
			else attron(COLOR_PAIR(2));
			printw("%c",board[i][j]);
		}
		printw("\n");
	}
	//printw("h: %d, w: %d", h, w);
	int h,w;
	getyx(stdscr,h,w);
	//move(h, w);
	refresh();
	//ch = getch();

    while((ch = getch()) != 'q'){
		switch (ch) {
			case KEY_LEFT:
				w--;
				break;
			case KEY_RIGHT:
				w++;
				break;
			case KEY_UP:
				h--;
				break;
			case KEY_DOWN:
				h++;
				break;
			case 10:
				switch (char_selected){
					case 0:
						select = mvinch(h, w) & A_CHARTEXT;
						char_selected = true;
						move(8, 0);
						printw("Char : %c", select);
						break;
					case 1:
						addch(select);
						char_selected = false;
						break;
				}
				
				break;

			//default: 

		}
		move(h, w);
		refresh();
	}
	
    

    // End ncurses
    endwin();



	return 0;
}
