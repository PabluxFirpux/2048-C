#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>


#define GRID 4
#define SQUARE_CHAR_HEIGHT 3
#define SQUARE_CHAR_WIDTH 6

typedef struct {
  int value;
  char* number;
  int numberCharLen;
} Cell;


int gridsize = GRID;

void init_color_pairs() {
  start_color();
  init_pair(1, COLOR_WHITE, COLOR_RED);
  init_pair(2, COLOR_WHITE, COLOR_GREEN);
  init_pair(3, COLOR_WHITE, COLOR_BLUE);
  init_pair(4, COLOR_WHITE, COLOR_BLACK);
  init_pair(5, COLOR_WHITE, COLOR_YELLOW);
  init_pair(0, COLOR_WHITE, COLOR_CYAN);
}

// Valid method to draw squares
void drawSquareAtPos(int x, int y, Cell cell) {
  attron(COLOR_PAIR(x%6));
  int startY = y*SQUARE_CHAR_HEIGHT;
  int startX = x*SQUARE_CHAR_WIDTH;
  for (int i = 0; i < SQUARE_CHAR_HEIGHT; i++) {
    for (int j = 0; j < SQUARE_CHAR_WIDTH; j++) {
      if (i != 1) {
        mvaddch(i+startY,j+startX,' ');
        continue;
      }
      if (cell.numberCharLen == 1 || cell.numberCharLen == 2) {
        if (j == 2 || j == 3) {
          mvprintw(i+startY,j+startX,cell.number);
          j+= cell.numberCharLen-1;
        } else {
          mvaddch(i+startY,j+startX,' ');
        }
      }

      if (cell.numberCharLen == 3 || cell.numberCharLen == 4) {
        if (j ==1) {
          mvprintw(i+startY,j+startX,cell.number);
          j+= cell.numberCharLen-1;
        } else {
          mvaddch(i+startY,j+startX,' ');
        }
      }
    }    
  }
  attroff(COLOR_PAIR(1));
}


void updateScreen(Cell** board, Cell cell) {
  drawSquareAtPos(0 , 0, cell);
  drawSquareAtPos(1, 0, cell);
  drawSquareAtPos(2, 0, cell);
  cell.numberCharLen = 3;
  cell.number = "323";
  drawSquareAtPos(3, 0, cell);
  drawSquareAtPos(4, 0, cell);
  cell.numberCharLen = 4;
  cell.number = "3232";
  drawSquareAtPos(5, 0, cell);
}


int main(int argc, char *argv[]) {
  Cell cellone;
  cellone.value = 2;
  cellone.number = "2";
  cellone.numberCharLen = 1;

  Cell** board = malloc(gridsize * sizeof(Cell) * gridsize); 

  size_t width, heigth;

  initscr();
  init_color_pairs();
  getmaxyx(stdscr, heigth, width);


	//printw("Hello World !!! W: %d, H: %d", width, heigth);
	refresh();
  updateScreen(board, cellone);
  refresh();
	getch();
	endwin();

	return 0;
    
}
