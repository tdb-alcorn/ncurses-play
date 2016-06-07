#include <ncurses.h>
#include <stdio.h>

int ROW, COL;
const int ESC = 27;

void ensureposition(int* y, int* x) {
    if (*x >= COL) {
        *x = COL - 1;
    } else if (*x < 0) {
        *x = 0;
    }
    if (*y >= ROW) {
        *y = ROW - 1;
    } else if (*y < 0) {
        *y = 0;
    }
}

int main() {
    int ch, x, y;
    char fname[80];
    FILE *fp;
    unsigned int *data;

    initscr();
    cbreak();
    noecho();
    keypad (stdscr, true);            /* Enable arrow keys.                  */

    getmaxyx(stdscr, ROW, COL);

    start_color();                    /* Yay colors!                         */
    init_pair(1, COLOR_RED, COLOR_WHITE);

    getyx(stdscr, y, x);

    // attron(COLOR_PAIR(1));

    while (true) {
        ch = getch();
        switch (ch) {
            case KEY_LEFT:
                x -= 1;
                break;
            case KEY_RIGHT:
                x += 1;
                break;
            case KEY_UP:
                y -= 1;
                break;
            case KEY_DOWN:
                y += 1;
                break;
            case ESC:
                ch = getch();
                switch (ch) {
                    case ESC:          /* Press ESC twice to exit.           */
                        endwin();
                        return 0;
                        break;
                    case 'w':
                        move(ROW-1, 0);
                        printw("Enter a file name (up to 80 characters): ");
                        //echo();
                        getstr(fname);
                        //noecho();
                        fp = fopen(fname, "w");
                        if (fp == NULL) {
                            perror("Cannot open file");
                        }
                        inchnstr(data, 1);
                        fputs((const char *)data, fp);
                        fclose(fp);
                        break;
                    default:
                        break;
                }
                break;
            default:
                addch(ch);
                //getyx(stdscr, y, x);
                break;
        }
        ensureposition(&y, &x);
        move(y, x);
    }
}

