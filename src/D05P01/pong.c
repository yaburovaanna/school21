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
#define WIN_SCORE 21

void run(void);
int choose_start_direction(void);
int read_secret_key(int player);
void clear_buffer(void);
int check_game_over(int left_rocket_score, int right_rocket_score);
void announce_winner(int is_left, int left_score, int right_score);
void render(int left_rocket_pos, int right_rocket_pos, int ball_x, int ball_y, int left_rocket_score,
            int right_rocket_score);
void clear_console(void);
void hide_cursor(void);
void print_top_border_line(int left_rocket_score, int right_rocket_score);
int is_rocket_segment(int x, int y, int rocket_x, int rocket_pos);
void print_border_line(void);
void show_cursor(void);
int is_valid_game_key(int c);
int get_valid_input(void);
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
    int need_continue = 0, left_rocket_pos = 11, right_rocket_pos = 11, ball_x = BALL_START_X,
        ball_y = BALL_START_Y, left_rocket_score = 0, right_rocket_score = 0;
    int ball_dx = choose_start_direction(), ball_dy = 1;

    while (need_continue == 0) {
        render(left_rocket_pos, right_rocket_pos, ball_x, ball_y, left_rocket_score, right_rocket_score);
        need_continue = check_game_over(left_rocket_score, right_rocket_score);
        int ch = get_valid_input();
        if (ch == EOF) {
            need_continue = 1;
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
        }
    }
}

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

void clear_buffer(void) {
    int tmp = getchar();
    while (tmp != '\n' && tmp != EOF) {
        tmp = getchar();
    }
}

int read_secret_key(int player) {
    int c;

    if (player == 1) {
        printf("Left player, press any key (secretly): ");
    } else {
        printf("Right player, press any key (secretly): ");
    }

    c = getchar();
    clear_buffer();
    return c;
}

int choose_start_direction(void) {
    int c1, result = 0;

    c1 = read_secret_key(1);
    if (c1 == EOF) {
        result = 1;
    } else {
        int c2 = read_secret_key(2);
        if (c2 == EOF) {
            result = 1;
        } else {
            if ((c1 + c2) % 2 == 0) {
                result = 1;
            } else {
                result = -1;
            }
        }
    }

    return result;
}

int check_game_over(int left_rocket_score, int right_rocket_score) {
    int game_over = 0;

    if (left_rocket_score >= WIN_SCORE) {
        announce_winner(1, left_rocket_score, right_rocket_score);
        game_over = 1;
    } else if (right_rocket_score >= WIN_SCORE) {
        announce_winner(0, left_rocket_score, right_rocket_score);
        game_over = 1;
    }

    return game_over;
}

void announce_winner(int is_left, int left_score, int right_score) {
    clear_console();

    if (is_left) {
        printf("Left player wins with score %d:%d! Congratulations!\n", left_score, right_score);
    } else {
        printf("Right player wins with score %d:%d! Congratulations!\n", left_score, right_score);
    }

    printf("Press any key to close the window.\n");
    getchar();
}

int is_rocket_segment(int x, int y, int rocket_x, int rocket_pos) {
    return x == rocket_x && y >= rocket_pos && y <= rocket_pos + 2;
}

void render(int left_rocket_pos, int right_rocket_pos, int ball_x, int ball_y, int left_rocket_score,
            int right_rocket_score) {
    int y, x;

    clear_console();
    hide_cursor();
    print_top_border_line(left_rocket_score, right_rocket_score);
    for (y = 0; y < HEIGHT; y++) {
        printf("#");
        for (x = 0; x < WIDTH; x++) {
            if (x == ball_x && y == ball_y) {
                printf("o");
            } else if (is_rocket_segment(x, y, LEFT_ROCKET_X, left_rocket_pos) ||
                       is_rocket_segment(x, y, RIGHT_ROCKET_X, right_rocket_pos) || x == MIDDLE_X) {
                printf("|");
            } else {
                printf(" ");
            }
        }
        printf("#\n");
    }
    print_border_line();
    show_cursor();
}

void clear_console(void) { printf("\033[2J\033[H"); }

void hide_cursor(void) { printf("\033[?25l"); }

void print_top_border_line(int left_rocket_score, int right_rocket_score) {
    int i;
    for (i = 0; i < BORDER_WIDTH; i++) {
        if (i == 36) {
            printf("%02d", left_rocket_score);
            i++;
        } else if (i == 43) {
            printf("%02d", right_rocket_score);
            i++;
        } else {
            printf("#");
        }
    }
    printf("\n");
}

void print_border_line(void) {
    int i;
    for (i = 0; i < BORDER_WIDTH; i++) {
        printf("#");
    }
    printf("\n");
}

void show_cursor(void) { printf("\033[?25h"); }

int is_valid_game_key(int c) {
    return c == 'a' || c == 'A' || c == 'z' || c == 'Z' || c == 'k' || c == 'K' || c == 'm' || c == 'M' ||
           c == ' ';
}

int get_valid_input(void) {
    int result;

    result = 0;
    while (result == 0) {
        int c = getchar();

        if (c == EOF) {
            result = EOF;
        } else if (is_valid_game_key(c)) {
            clear_buffer();
            result = c;
        } else {
            if (c != '\n') {
                clear_buffer();
            }
            printf("Invalid character entered. Continue playing? (Y/N): ");
            c = getchar();
            if (c == 'Y' || c == 'y') {
                clear_buffer();
            } else {
                result = EOF;
            }
        }
    }
    return result;
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