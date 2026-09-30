#include <ncurses.h>
#include <stdbool.h>

#define CELLS_X 20
#define CELLS_Y 20

#define TICKS 25000
typedef enum {DEAD, ALIVE} CellStatus;

void builder(CellStatus (*cells)[CELLS_X][CELLS_Y]) {
    int key = 0;

    struct {
        int x;
        int y;
    } current_pos;
    current_pos.x = 0;
    current_pos.y = 0;

    move(CELLS_Y+2, 0);
    printw("ARROWS - move, SPACE - place/destroy, ENTER - run\n");

    while (key != 10) {

        switch (key) {
            case KEY_UP:
                if (current_pos.y > 0)
                    current_pos.y--;
                break;
            case KEY_DOWN:
                if (current_pos.y < CELLS_Y-1)
                    current_pos.y++;
                break;
            case KEY_LEFT:
                if (current_pos.x > 0)
                    current_pos.x--;
                break;
            case KEY_RIGHT:
                if (current_pos.x < CELLS_X-1)
                    current_pos.x++;
                break;
            case 32:
                move(current_pos.y, current_pos.x);
                if ((*cells)[current_pos.x][current_pos.y] == DEAD) {
                    printw("@");
                    (*cells)[current_pos.x][current_pos.y] = ALIVE;
                } else {
                    printw(" ");
                    (*cells)[current_pos.x][current_pos.y] = DEAD;
                }
            default: break;
        }

        move(current_pos.y, current_pos.x);

        refresh();
        key = getch();
    }
}

int main() {

    CellStatus cells[CELLS_X][CELLS_Y] = {DEAD};
    //cells[8][8] = ALIVE;
    //cells[9][8] = ALIVE;
    //cells[10][8] = ALIVE;
    //cells[10][7] = ALIVE;
    //cells[10][6] = ALIVE;
    //cells[10][5] = ALIVE;

    initscr();
    noecho();
    start_color();
    //init_pair(1, COLOR_RED, COLOR_BLACK);
    keypad(stdscr, true);

    builder(&cells);
    nodelay(stdscr, true);

    struct {
        bool paused;
        bool next;
    } pause;
    pause.paused = false;
    pause.next = false;
    while (1) {
        static int tick = TICKS;

        if (tick == TICKS || pause.next) {
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
            pause.next = false;
        }
        if (!pause.paused) {
            tick++;
        }
        refresh();

        switch(getch()) {
            case 32:
                pause.paused = !pause.paused;
                tick = 0;
                break;
            case 110:
                if (pause.paused)
                    pause.next = true;
                break;
            default: break;
        }

        move(CELLS_Y+2, 0);
        if (!pause.paused)
            printw("SPACE - pause\n");
        else
            printw("SPACE - unpause, N - next\n");
        move(CELLS_Y+3, 0);
        printw("Ticks: %d\n", tick);
    }

    //getch();
    endwin();

    return 0;
}
