#include <ncurses.h>
#include <stdbool.h>

#define CELLS_X 20
#define CELLS_Y 20

#define TICKS 25000


int main() {

    typedef enum {DEAD, ALIVE} CellStatus;
    CellStatus cells[CELLS_X][CELLS_Y] = {DEAD};
    cells[8][8] = ALIVE;
    cells[9][8] = ALIVE;
    cells[10][8] = ALIVE;
    cells[10][7] = ALIVE;
    cells[10][6] = ALIVE;
    cells[10][5] = ALIVE;

    initscr();
    noecho();
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);

    while (1) {
        static int tick = TICKS;

        if (tick == TICKS) {
            bool change[CELLS_X][CELLS_Y] = {false};

            for (int x=0; x<CELLS_X; x++) {
                for (int y=0; y<CELLS_Y; y++) {

                    typedef struct {
                        int x;
                        int y;
                    } Pos;
                    Pos positions[8] = {
                    {x+1, y},
                    {x-1, y},
                    {x, y+1},
                    {x, y-1},

                    {x+1, y+1},
                    {x+1, y-1},
                    {x-1, y+1},
                    {x-1, y-1}
                    };

                    int neighbours = 0;
                    for (int i=0; i<8; i++) {
                        Pos pos = positions[i];
                        if (pos.x < CELLS_X && pos.y < CELLS_Y && pos.x >= 0 && pos.y >= 0) {
                            if (cells[pos.x][pos.y] == ALIVE)
                                neighbours++;
                        }
                    }
                    move(y, x);
                    if (cells[x][y] == ALIVE) {
                        printw("@");
                        //printw("%d", neighbours);
                    } else {
                        printw(" ");
                    }

                    if (cells[x][y] == ALIVE) {
                        if (neighbours < 2 || neighbours > 3)
                            change[x][y] = true;
                    } else {
                        if (neighbours == 3)
                            change[x][y] = true;
                    }

                }

            }

            for (int x=0; x<CELLS_X; x++) {
                for (int y=0; y<CELLS_Y; y++) {
                    if (change[x][y]) {
                        cells[x][y] = (cells[x][y] == ALIVE) ? DEAD : ALIVE;
                    }
                }
            }

            tick = 0;
        }
        tick++;
        refresh();

        move(22, 0);
        printw("Ticks: %d\n", tick);
    }

    //getch();
    endwin();

    return 0;
}
