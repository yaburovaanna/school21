#include <ncurses.h>
#include <stdio.h>

#define ROWS 25
#define COLS 80
#define START_DELAY 250
#define MIN_DELAY 50
#define MAX_DELAY 1000
#define DELAY_STEP 50

int read_board(int board[ROWS][COLS]);
int read_cell(int *value);
int is_separator(int symbol);
void run_game(int board[ROWS][COLS]);
void draw_board(int board[ROWS][COLS]);
int handle_key(int key, int delay_ms, int *running);
void update_board(int board[ROWS][COLS]);
int count_neighbors(int board[ROWS][COLS], int row, int col);
int wrap_index(int value, int limit);
int next_cell_state(int current, int neighbors);

int main(void) {
    int board[ROWS][COLS] = {0};
    int input_ok = read_board(board);

    if (input_ok) {
        run_game(board);
    }

    return 0;
}

int read_board(int board[ROWS][COLS]) {
    int valid = 1;

    for (int row = 0; row < ROWS && valid; row++) {
        for (int col = 0; col < COLS && valid; col++) {
            valid = read_cell(&board[row][col]);
        }
    }

    return valid;
}

int read_cell(int *value) {
    int symbol = getchar();
    int valid = 1;

    while (symbol != EOF && is_separator(symbol)) {
        symbol = getchar();
    }
    if (symbol == '0' || symbol == '1') {
        *value = symbol - '0';
    } else {
        valid = 0;
    }

    return valid;
}

int is_separator(int symbol) {
    int result = symbol == ' ' || symbol == '\n' || symbol == '\t' || symbol == '\r';

    return result;
}

void run_game(int board[ROWS][COLS]) {
    FILE *keyboard = fopen("/dev/tty", "r");
    SCREEN *screen = NULL;
    int delay_ms = START_DELAY;
    int running = 1;

    if (keyboard != NULL) {
        screen = newterm(NULL, stdout, keyboard);
    }
    if (screen != NULL) {
        set_term(screen);
        cbreak();
        noecho();
        curs_set(0);
        timeout(delay_ms);
        while (running) {
            draw_board(board);
            int key = getch();
            delay_ms = handle_key(key, delay_ms, &running);
            if (running) {
                update_board(board);
                timeout(delay_ms);
            }
        }
        endwin();
        delscreen(screen);
    }
    if (keyboard != NULL) {
        fclose(keyboard);
    }
}

void draw_board(int board[ROWS][COLS]) {
    erase();
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {
            char symbol = board[row][col] ? '*' : ' ';
            mvaddch(row, col, (chtype)symbol);
        }
    }
    refresh();
}

int handle_key(int key, int delay_ms, int *running) {
    if (key == 'A' && delay_ms > MIN_DELAY) {
        delay_ms -= DELAY_STEP;
    } else if (key == 'Z' && delay_ms < MAX_DELAY) {
        delay_ms += DELAY_STEP;
    } else if (key == ' ') {
        *running = 0;
    }

    return delay_ms;
}

void update_board(int board[ROWS][COLS]) {
    int next[ROWS][COLS] = {0};

    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {
            int neighbors = count_neighbors(board, row, col);
            next[row][col] = next_cell_state(board[row][col], neighbors);
        }
    }
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {
            board[row][col] = next[row][col];
        }
    }
}

int count_neighbors(int board[ROWS][COLS], int row, int col) {
    int count = 0;

    for (int row_shift = -1; row_shift <= 1; row_shift++) {
        for (int col_shift = -1; col_shift <= 1; col_shift++) {
            if (row_shift != 0 || col_shift != 0) {
                int neighbor_row = wrap_index(row + row_shift, ROWS);
                int neighbor_col = wrap_index(col + col_shift, COLS);
                count += board[neighbor_row][neighbor_col];
            }
        }
    }

    return count;
}

int wrap_index(int value, int limit) {
    int result = value;

    if (result < 0) {
        result = limit - 1;
    } else if (result >= limit) {
        result = 0;
    }

    return result;
}

int next_cell_state(int current, int neighbors) {
    int result = 0;

    if ((current == 1 && (neighbors == 2 || neighbors == 3)) || (current == 0 && neighbors == 3)) {
        result = 1;
    }

    return result;
}
