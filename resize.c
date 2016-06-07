#include <ncurses.h>
#include <strings.h>

int main() {
    char msg[] = "Some string!";
    int row, col;

    initscr();
    getmaxyx(stdscr, row, col);
    mvprintw(row/2, (col-strlen(msg))/2, "%s", msg);
    addch('x' | A_BOLD | A_UNDERLINE);

    mvprintw(row-2,0,"This screen has %d rows and %d columns\n",row,col);
    printw("Try resizing and re-running");
    refresh();
    getch();
    endwin();

    return 0;
}
