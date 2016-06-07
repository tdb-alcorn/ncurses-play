#include <ncurses.h>

int main() {
    int ch;

    initscr();
    raw();
    keypad(stdscr, true);
    noecho();

    printw("type something to see it in bold\n");
    ch =getch();

    if (ch == KEY_F(1)) {
        printw("you pressed f1 you whore");
    } else {
        printw("you pressed ");
        attron(A_BOLD);
        printw("%c", ch);
        attroff(A_BOLD);
    }

    refresh();
    getch();
    endwin();

    return 0;
}
