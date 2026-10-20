#include <ncurses.h>
#include <stdio.h>

#define WIDTH 80
#define BORDER_WIDTH 82
#define HEIGHT 25

#define LEFT_ROCKET_X 2
#define RIGHT_ROCKET_X 77
#define MIDDLE_X 39
#define ROCKET_HEIGHT 3
#define BALL_START_X 39
#define BALL_START_Y 12
#define ROCKET_START_Y 11
#define WIN_SCORE 21
#define FRAME_DELAY_MS 100

void run(void);
void init_screen(void);
void close_screen(void);
int is_quit_key(int c);
int check_game_over(int left_rocket_score, int right_rocket_score);
void announce_winner(int is_left, int left_score, int right_score);
void finish_game(int left_rocket_score, int right_rocket_score);
void render(int left_rocket_pos, int right_rocket_pos, int ball_x, int ball_y, int left_rocket_score,
            int right_rocket_score);
void print_top_border_line(int left_rocket_score, int right_rocket_score);
void print_border_line(void);
int is_rocket_segment(int x, int y, int rocket_x, int rocket_pos);
int update_rocket(int rocket_y, char c, char up, char down);
int top_bottom_bounce_y(int ball_y);
int top_bottom_bounce_dy(int ball_y, int ball_dy);
int rocket_bounce_x(int ball_x, int ball_dx, int ball_y, int left_rocket_pos, int right_rocket_pos);
int rocket_bounce_dx(int ball_x, int ball_dx, int ball_y, int left_rocket_pos, int right_rocket_pos);

int main(void) {
    run();
    return 0;
}

void run(void) {
    int game_over = 0, left_rocket_pos = ROCKET_START_Y, right_rocket_pos = ROCKET_START_Y,
        ball_x = BALL_START_X, ball_y = BALL_START_Y, left_rocket_score = 0, right_rocket_score = 0;
    int ball_dx = 1, ball_dy = 1;

    init_screen();
    while (game_over == 0) {
        render(left_rocket_pos, right_rocket_pos, ball_x, ball_y, left_rocket_score, right_rocket_score);
        int ch = getch();
        flushinp();
        if (is_quit_key(ch)) {
            game_over = 1;
        } else {
            left_rocket_pos = update_rocket(left_rocket_pos, (char)ch, 'a', 'z');
            right_rocket_pos = update_rocket(right_rocket_pos, (char)ch, 'k', 'm');
            ball_x += ball_dx;
            ball_y += ball_dy;
            ball_y = top_bottom_bounce_y(ball_y);
            ball_dy = top_bottom_bounce_dy(ball_y, ball_dy);
            ball_dx = rocket_bounce_dx(ball_x, ball_dx, ball_y, left_rocket_pos, right_rocket_pos);
            ball_x = rocket_bounce_x(ball_x, ball_dx, ball_y, left_rocket_pos, right_rocket_pos);
            if (ball_x < 0) {  // goals
                right_rocket_score = right_rocket_score + 1;
                ball_x = BALL_START_X;
                ball_y = BALL_START_Y;
                ball_dx = -1;
                ball_dy = 1;
            }
            if (ball_x > WIDTH - 1) {
                left_rocket_score = left_rocket_score + 1;
                ball_x = BALL_START_X;
                ball_y = BALL_START_Y;
                ball_dx = 1;
                ball_dy = 1;
            }
            game_over = check_game_over(left_rocket_score, right_rocket_score);
            napms(FRAME_DELAY_MS);
        }
    }
    finish_game(left_rocket_score, right_rocket_score);
    close_screen();
}

void init_screen(void) {
    initscr();
    cbreak();
    noecho();
    nodelay(stdscr, TRUE);
    curs_set(0);
}

void close_screen(void) {
    curs_set(1);
    endwin();
}

int is_quit_key(int c) { return c == 'q' || c == 'Q'; }

int top_bottom_bounce_y(int ball_y) {
    int result = ball_y;
    if (ball_y < 0) {  // top
        result = 1;
    }
    if (ball_y > 24) {  // bottom
        result = 23;
    }
    return result;
}

int top_bottom_bounce_dy(int ball_y, int ball_dy) {
    int result = ball_dy;
    if (ball_y < 1) {  // top
        result = 1;
    }
    if (ball_y >= 24) {  // bottom
        result = -1;
    }
    return result;
}

int rocket_bounce_x(int ball_x, int ball_dx, int ball_y, int left_rocket_pos, int right_rocket_pos) {
    int result = ball_x;
    if (ball_x == LEFT_ROCKET_X && ball_dx < 0 && ball_y >= left_rocket_pos &&  // rockets
        ball_y <= left_rocket_pos + ROCKET_HEIGHT - 1) {
        result = LEFT_ROCKET_X + 2;
    }
    if (ball_x == RIGHT_ROCKET_X && ball_dx > 0 && ball_y >= right_rocket_pos &&
        ball_y <= right_rocket_pos + ROCKET_HEIGHT - 1) {
        result = RIGHT_ROCKET_X - 2;
    }
    return result;
}

int rocket_bounce_dx(int ball_x, int ball_dx, int ball_y, int left_rocket_pos, int right_rocket_pos) {
    int result = ball_dx;
    if (ball_x == LEFT_ROCKET_X + 1 && ball_dx < 0 && ball_y >= left_rocket_pos &&  // rockets
        ball_y <= left_rocket_pos + ROCKET_HEIGHT - 1) {
        result = 1;
    }
    if (ball_x == RIGHT_ROCKET_X - 1 && ball_dx > 0 && ball_y >= right_rocket_pos &&
        ball_y <= right_rocket_pos + ROCKET_HEIGHT - 1) {
        result = -1;
    }
    return result;
}

int check_game_over(int left_rocket_score, int right_rocket_score) {
    return left_rocket_score >= WIN_SCORE || right_rocket_score >= WIN_SCORE;
}

void finish_game(int left_rocket_score, int right_rocket_score) {
    if (left_rocket_score >= WIN_SCORE) {
        announce_winner(1, left_rocket_score, right_rocket_score);
    } else if (right_rocket_score >= WIN_SCORE) {
        announce_winner(0, left_rocket_score, right_rocket_score);
    }
}

void announce_winner(int is_left, int left_score, int right_score) {
    erase();

    if (is_left) {
        printw("Left player wins with score %d:%d! Congratulations!\n", left_score, right_score);
    } else {
        printw("Right player wins with score %d:%d! Congratulations!\n", left_score, right_score);
    }

    printw("Press any key to close the window.\n");
    refresh();
    nodelay(stdscr, FALSE);
    getch();
}

int is_rocket_segment(int x, int y, int rocket_x, int rocket_pos) {
    return x == rocket_x && y >= rocket_pos && y <= rocket_pos + ROCKET_HEIGHT - 1;
}

void render(int left_rocket_pos, int right_rocket_pos, int ball_x, int ball_y, int left_rocket_score,
            int right_rocket_score) {
    int y, x;

    erase();
    print_top_border_line(left_rocket_score, right_rocket_score);
    for (y = 0; y < HEIGHT; y++) {
        printw("#");
        for (x = 0; x < WIDTH; x++) {
            if (x == ball_x && y == ball_y) {
                printw("o");
            } else if (is_rocket_segment(x, y, LEFT_ROCKET_X, left_rocket_pos) ||
                       is_rocket_segment(x, y, RIGHT_ROCKET_X, right_rocket_pos) || x == MIDDLE_X) {
                printw("|");
            } else {
                printw(" ");
            }
        }
        printw("#\n");
    }
    print_border_line();
    refresh();
}

void print_top_border_line(int left_rocket_score, int right_rocket_score) {
    int i;
    for (i = 0; i < BORDER_WIDTH; i++) {
        if (i == 36) {
            printw("%02d", left_rocket_score);
            i++;
        } else if (i == 43) {
            printw("%02d", right_rocket_score);
            i++;
        } else {
            printw("#");
        }
    }
    printw("\n");
}

void print_border_line(void) {
    int i;
    for (i = 0; i < BORDER_WIDTH; i++) {
        printw("#");
    }
    printw("\n");
}

int update_rocket(int rocket_y, char c, char up, char down) {
    int new_y = rocket_y;

    if (c == up || c == up - 32) {
        if (rocket_y > 0) {
            new_y = rocket_y - 1;
        }
    } else if (c == down || c == down - 32) {
        if (rocket_y < 22) {
            new_y = rocket_y + 1;
        }
    }

    return new_y;
}