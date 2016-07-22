#include <curses.h>
#include <stdio.h>
#include <stdlib.h>

/* TODO
 *
 * Deleted characters should not be written to file on save
 * Alternaitvely, how to record which characters are deleted so they can be
 * displayed as deleted when the file is reopened?
 */

int ROW, COL;
const int ESC = 27;
const int BACKSPACE = 127;

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

int string_length(char* string) {
    int length = 0;
    while (string[length] != '\0') {
        length++;
    }
    return length;
}

int fill_nulls(char* array, char fill_char, int capacity) {
    int string_end = 0;
    for (int i=capacity-1; i>=0; --i) {
        if (string_end == 0 && array[i] != '\0') {
            string_end = i+1;
        }
        if (string_end > 0 && array[i] == '\0') {
            array[i] = fill_char;
        }
    }
    return string_end;
}

void contents_to_file(char** contents, FILE* fp, int num_rows, int num_columns) {
    int string_length = 0;
    for (int i=0; i<num_rows; i++) {
        string_length = fill_nulls(contents[i], ' ', num_columns);
        fputs(contents[i], fp);
        if (string_length>0) {
            fputc('\r', fp);
            fputc('\n', fp);
        }
    }
}

void print_all_chars(char** contents, int num_strings, int string_width) {
    for (int row=0; row<num_strings; row++) {
        for (int col=0; col<string_width; col++) {
            if (contents[row][col] == '\0') {
                putchar('u');
            } else {
                putchar(contents[row][col]);
            }
        }
        putchar('\r');
        putchar('\n');
    }
}

char** new_string_table(int num_strings, int string_width) {
    char* _ = (char*)calloc(num_strings, string_width*sizeof(char));
    char** string_table = (char**)calloc(num_strings, sizeof(char*));
    for (int i=0; i<num_strings; i++) {
        string_table[i] = _ + (i * string_width * sizeof(char));
    }
    return string_table;
}

void delete_string_table(char** string_table, int num_strings) {
    free(string_table[0]);  /* Free memory block containing strings. */
    free(string_table);  /* Free memory block containing pointers to strings. */
}

int main() {
    int ch, x, y;
    char fname[80];
    FILE *fp;
    bool entering_metadata = false;
    char to_delete;
    char existing;

    initscr();
    cbreak();
    noecho();

    nonl();
    intrflush(stdscr, false);
    keypad(stdscr, true);             /* Enable arrow keys.                  */

    getmaxyx(stdscr, ROW, COL);

    move(0,0);
    start_color();                    /* Yay colors!                         */
    init_pair(1, COLOR_RED, COLOR_WHITE);
    // attron(COLOR_PAIR(1));
    // printw("rows: %d, columns: %d\n", ROW, COL);

    char** contents = new_string_table(ROW, COL);

    char* blank_row = (char*)malloc(COL*sizeof(char));
    for (int i=0; i<COL; i++) {
        blank_row[i] = ' ';
    }


    getyx(stdscr, y, x);


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
            case BACKSPACE:  /* KEY_BACKSPACE wasn't working. */
                --x;
                move(y, x);
                to_delete = inch();
                attron(COLOR_PAIR(1));
                addch(to_delete);
                attroff(COLOR_PAIR(1));
                break;
            case KEY_DC:
                //attron(COLOR_PAIR(1));
                addch('X');
                //attroff(COLOR_PAIR(1));
                break;
            case ESC:
                ch = getch();
                switch (ch) {
                    case ESC:          /* Press ESC twice to exit.           */
                        endwin();
                        free(blank_row);
                        delete_string_table(contents, ROW);
                        return 0;
                        break;
                    case 'w':
                        move(ROW-1, 0);
                        printw("Enter a file name (up to 80 characters): ");
                        echo();
                        if (getnstr(fname, 80) == ERR) {
                            perror("Something went wrong reading the file name you gave.");
                        }
                        noecho();
                        fp = fopen(fname, "w");
                        if (fp == NULL) {
                            perror("Cannot open file");
                        }
                        contents_to_file(contents, fp, ROW, COL);
                        fclose(fp);
                        move(ROW-1, 0);
                        printw(blank_row);
                        break;
                    default:
                        break;
                }
                break;
            default:
                existing = contents[y][x];
                //printw("%d", (int)existing);
                if (existing == '\0') {
                    addch(ch);
                    ensureposition(&y, &x);
                    contents[y][x] = ch;
                    ++x;
                }
                //getyx(stdscr, y, x);
                break;
        }
        ensureposition(&y, &x);
        move(y, x);
    }
}

