#include <ncurses.h>

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

    initscr();
    cbreak();
    noecho();
    keypad (stdscr, true);            /* Enable arrow keys.                  */

    getmaxyx(stdscr, ROW, COL);

    start_color();                    /* Yay colors!                         */
    init_pair(1, COLOR_RED, COLOR_WHITE);


    attron(COLOR_PAIR(1));

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
                    default:
                        break;
                }
                break;
            default:
                addch(ch);
                getyx(stdscr, y, x);
                break;
        }
        ensureposition(&y, &x);
        move(y, x);
    }
}

