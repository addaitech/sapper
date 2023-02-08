#include <ncurses.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>

#define LEFT_PANE 1
#define RIGHT_PANE 2

WINDOW *left_pane, *right_pane;

void init_ncurses() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
}

void create_windows() {
    int height, width;
    getmaxyx(stdscr, height, width);

    left_pane = newwin(height, width / 2, 0, 0);
    right_pane = newwin(height, width / 2, 0, width / 2);
}

void display_directory(WINDOW *win, char *path) {
    DIR *dir = opendir(path);
    struct dirent *entry;

    wclear(win);
    while ((entry = readdir(dir)) != NULL) {
        wprintw(win, "%s\n", entry->d_name);
    }
    closedir(dir);
    wrefresh(win);
}

void handle_user_input(int *current_pane) {
    int ch = getch();
    switch (ch) {
        case KEY_LEFT:
            *current_pane = LEFT_PANE;
            break;
        case KEY_RIGHT:
            *current_pane = RIGHT_PANE;
            break;
        case 'q':
            endwin();
            exit(0);
            break;
    }
}

int main() {
    init_ncurses();
    create_windows();

    int current_pane = LEFT_PANE;
    char left_path[256] = "/";
    char right_path[256] = "/";

    while (1) {
        if (current_pane == LEFT_PANE) {
            display_directory(left_pane, left_path);
        } else {
            display_directory(right_pane, right_path);
        }
        handle_user_input(&current_pane);
    }

    endwin();
    return 0;
}
