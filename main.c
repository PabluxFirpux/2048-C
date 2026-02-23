#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>


#define GRID 4
#define SQUARE_CHAR_HEIGHT 3
#define SQUARE_CHAR_WIDTH 6

typedef struct {
  int index;
  char* number;
  int numberCharLen;
} Cell;


int gridsize = GRID;

Cell* getCellByIndex(int index) {
  if (index < 1 || index > 11) {
    printf("Incorrect usage");
    exit(1);
  }
  Cell* pcell = calloc(1, sizeof(Cell*));
  pcell->index = index;
  switch (index) {
    case 1: 
      pcell->number = "2 ";
      pcell->numberCharLen = 2;
      break;
    case 2:
      pcell->number = "4 ";
      pcell->numberCharLen = 2;
      break;
    case 3:
      pcell->number = "8 ";
      pcell->numberCharLen = 2;
      break;
    case 4:
      pcell->number = "16";
      pcell->numberCharLen = 2;
      break;
    case 5:
      pcell->number = "32";
      pcell->numberCharLen = 2;
      break;
    case 6:
      pcell->number = "64";
      pcell->numberCharLen = 2;
      break;
    case 7:
      pcell->number = "128";
      pcell->numberCharLen = 3;
      break;
    case 8:
      pcell->number = "256";
      pcell->numberCharLen = 3;
      break;
    case 9:
      pcell->number = "512";
      pcell->numberCharLen = 3;
      break;
    case 10:
      pcell->number = "1024";
      pcell->numberCharLen = 4;
      break;
    case 11:
      pcell->number = "2048";
      pcell->numberCharLen = 4;
      break;
  }
  return pcell;
}

void init_color_pairs() {
  start_color();
  init_pair(1, COLOR_WHITE, COLOR_RED);
  init_pair(2, COLOR_BLACK, COLOR_GREEN);
  init_pair(3, COLOR_WHITE, COLOR_BLUE);
  init_pair(4, COLOR_WHITE, COLOR_BLACK);
  init_pair(5, COLOR_WHITE, COLOR_YELLOW);
  init_pair(0, COLOR_WHITE, COLOR_RED);
}

// Valid method to draw squares
void drawSquareAtPos(int x, int y, Cell* cell) {
 // printf("%d %s %d", cell.index, cell.number, cell.numberCharLen);
  int padding = 1;
  attron(COLOR_PAIR((x+y)%6));
  int startY = y*SQUARE_CHAR_HEIGHT;
  int startX = x*SQUARE_CHAR_WIDTH;
  for (int i = 0; i < SQUARE_CHAR_HEIGHT; i++) {
    for (int j = 0; j < SQUARE_CHAR_WIDTH; j++) {
      if (i != 1) {
        mvaddch(i+startY,j+startX,' ');
        continue;
      }
      if (cell->numberCharLen == 1 || cell->numberCharLen == 2) {
        if (j == 2 || j == 3) {
          mvprintw(i+startY,j+startX,cell->number);
          j+= cell->numberCharLen-1;
        } else {
          mvaddch(i+startY,j+startX,' ');
        }
      }

      if (cell->numberCharLen == 3 || cell->numberCharLen == 4) {
        if (j ==1) {
          mvprintw(i+startY,j+startX,cell->number);
          j+= cell->numberCharLen-1;
        } else {
          mvaddch(i+startY,j+startX,' ');
        }
      }
    }    
  }
  attroff(COLOR_PAIR((x+y)%6));
}


void updateScreen(Cell*** board) {
  for (int x = 0; x< gridsize; x++) {
    for (int y = 0; y < gridsize; y++) {
      if (!board[x][y]) {
        continue;
      }
      drawSquareAtPos(y,x, board[x][y]);
    }
  }
}

void merge(int x1, int y1, int x2, int y2, Cell*** board) {
  if (!board[y1][x1] || !board[y2][x2]) {
    printf("No ambos son scuared\n");
    return;
  }
  if (board[y1][x1]->index == board[y2][x2]->index) {
    Cell* second = board[y2][x2];
    board[y2][x2] = NULL;
    //free(second);
    Cell* first = board[y1][x1];
    board[y1][x1] = getCellByIndex((first->index)+1);
    //free(first);
    return;
  }
}

void moveCellLeft(int x, int y, Cell*** board) {
  if (!board[y][x]) {
    return;
  }
  if (x-1 < 0) {
    return;
  }
  if (x-1 >= 0 && board[y][x-1] && board[y][x-1]->index == board[y][x]->index) {
    merge(x-1, y, x, y, board);
    return;
  }

  if (x-1 >= 0 && !board[y][x-1]) {
    Cell* cell = board[y][x];
    board[y][x] = NULL;
    board[y][x-1] = cell;
    return;
  }
}

void moveCellDown(int x, int y, Cell*** board) {
  if (!board[y][x]) {
    return;
  }

  if (y+1 >= gridsize) {
    return;
  }
  if (y+1 < gridsize && board[y+1][x] && board[y+1][x]->index == board[y][x]->index) {
    merge(x, y+1, x, y, board);
    return;
  }
  if (y+1 < gridsize && !board[y+1][x]) {
    Cell* cell = board[y][x];
    board[y][x] = NULL;
    board[y+1][x] = cell;
  }
}

void moveCellRight(int x, int y, Cell*** board) {
  if (!board[y][x]) {
    return;
  }

  if (x+1 >= gridsize) {
    return;
  }
  if (x+1 < gridsize && board[y][x+1] && board[y][x+1]->index == board[y][x]->index) {
    merge(x+1, y, x, y, board);
    return;
  }

  if (x+1 < gridsize) {
    Cell* cell = board[y][x];
    board[y][x] = NULL;
    board[y][x+1] = cell;
  }
}

void moveCellUp(int x, int y, Cell*** board) {
  if (!board[y][x]) {
    return;
  }

  if (y-1 < 0) {
    return;
  }

  if (y-1 >= 0 && board[y-1][x] && board[y-1][x]->index == board[y][x]->index) {
    merge(x, y-1, x, y, board);
    return;
  }

  if (y-1 >= 0) {
    Cell* cell = board[y-1][x];
    board[y][x] = NULL;
    board[y-1][x] = cell;
  }
}

void moveBoardRight(Cell*** board) {
  for (int y=0; y< gridsize; y++) {
    for (int x = gridsize-1; x>= 0;x--) {
      if (!board[y][x]) {
        continue;
      }
      moveCellRight(x,y,board);
    }
  }
}

int main(int argc, char *argv[]) {
  Cell*** board = calloc(gridsize, sizeof(Cell**)); 
  for (int i = 0; i<gridsize; i++) {
    board[i] = calloc(gridsize, sizeof(Cell*));
  }
  size_t width, heigth;

  initscr();
  init_color_pairs();
  getmaxyx(stdscr, heigth, width);
  Cell* cellone = getCellByIndex(1);
  Cell* celloned = getCellByIndex(2);
  Cell* cellonet = getCellByIndex(3);
  Cell* celltwo = getCellByIndex(4);
  Cell* cellthree = getCellByIndex(6);
  board[0][0] = cellone;
  board[0][3] = cellthree;
  board[1][0] = celloned;
  board[2][0] = cellthree;
  board[0][1] = celloned;
  board[0][2] = cellonet;
  board[1][1] = celloned;
	//printw("Hello World !!! W: %d, H: %d", width, heigth);
	refresh();
  updateScreen(board);
  refresh();
	getch();
  erase();
  refresh();
  //moveCellRight(0,1,board);
  moveBoardRight(board);
  updateScreen(board);
  refresh();
  getch();
  refresh();
  getch();


  for (int i=0; i< gridsize; i++) {
    free(board[i]);
  }

  free(board);
	endwin();

	return 0;
    
}
