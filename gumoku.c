#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <windows.h>  

#define MAX_ROW 20
#define MAX_COL 20
#define WHITE -1
#define BLACK 1
#define BLANK 0

typedef struct {
    int x;
    int y;
    int player;
} Move;

void draw_chessboard(int row, int col, int chessboard[][MAX_COL], int cursor_x, int cursor_y, int black_time, int white_time, int timer_enabled);
void draw_chessman(int x, int y, int type, char *tableline, int cursor_x, int cursor_y);
void draw_menu(void);
void end_menu(int *choice);
void person_person(int board_size, int win_condition, int allow_undo, int timer_enabled, int time_limit);
void customize_game(int *board_size, int *win_condition, int *allow_undo, int *timer_enabled, int *time_limit);
int is_full(int chessboard[][MAX_COL], int row, int col);
int is_win(int chessboard[][MAX_COL], int row, int col, int win_condition);
void move_cursor(int chessboard[][MAX_COL], int *cursor_x, int *cursor_y, int direction, int board_size);
void place_chess(int chessboard[][MAX_COL], int cursor_x, int cursor_y, int player, Move *history, int *move_count);
void undo_move(int chessboard[][MAX_COL], Move *history, int *move_count);
int check_time_limit(clock_t start_time, int time_limit);
void handle_end_game(int board_size, int win_condition, int allow_undo, int timer_enabled, int time_limit);

int main() {
    int choice;
    int board_size = 15, win_condition = 5, allow_undo = 0;
    int timer_enabled = 0, time_limit = 60; 
    
    draw_menu();
    scanf("%d", &choice);
    while (1) {
        switch (choice) {
            case 1:
                person_person(board_size, win_condition, allow_undo, timer_enabled, time_limit);
                break;
            case 2:
                customize_game(&board_size, &win_condition, &allow_undo, &timer_enabled, &time_limit);
                choice = 1;
                break;
            case 3:
                exit(0);
                break;
            default:
                printf("Invalid option, please try again.\n");
                printf("Please select: ");
                scanf("%d", &choice);
                break;
        }
    }
    return 0;
}

void draw_chessboard(int row, int col, int chessboard[][MAX_COL], int cursor_x, int cursor_y, int black_time, int white_time, int timer_enabled) {
    printf("\033[H\033[J");

    for (int i = 0; i < row; i++) {
        if (i == 0) {
            for (int j = 0; j < col; j++) {
                if (j == 0)
                    draw_chessman(i, j, chessboard[i][j], "┌ ", cursor_x, cursor_y);
                else if (j == col - 1)
                    draw_chessman(i, j, chessboard[i][j], "┐ ", cursor_x, cursor_y);
                else
                    draw_chessman(i, j, chessboard[i][j], "┬ ", cursor_x, cursor_y);
            }
            printf("\n");
        } else if (i == row - 1) {
            for (int j = 0; j < col; j++) {
                if (j == 0)
                    draw_chessman(i, j, chessboard[i][j], "└ ", cursor_x, cursor_y);
                else if (j == col - 1)
                    draw_chessman(i, j, chessboard[i][j], "┘ ", cursor_x, cursor_y);
                else
                    draw_chessman(i, j, chessboard[i][j], "┴ ", cursor_x, cursor_y);
            }
            printf("\n");
        } else {
            for (int j = 0; j < col; j++) {
                if (j == 0)
                    draw_chessman(i, j, chessboard[i][j], "├ ", cursor_x, cursor_y);
                else if (j == col - 1)
                    draw_chessman(i, j, chessboard[i][j], "┤ ", cursor_x, cursor_y);
                else
                    draw_chessman(i, j, chessboard[i][j], "┼ ", cursor_x, cursor_y);
            }
            printf("\n");
        }
    }

    if (timer_enabled) {
        printf("Black remaining time: %d sec\n", black_time);
        printf("White remaining time: %d sec\n", white_time);
    }
}

void draw_chessman(int x, int y, int type, char *tableline, int cursor_x, int cursor_y) {
    if (x == cursor_x && y == cursor_y)
        printf("+ ");
    else if (type == WHITE)
        printf("● ");
    else if (type == BLACK)
        printf("○ ");
    else if (type == BLANK)
        printf("%s", tableline);
}

void draw_menu(void) {
    printf("********************************\n");
    printf("******     Gomoku Game    ******\n");
    printf("******      Game Mode     ******\n");
    printf("*      1. Classic Mode         *\n");
    printf("*      2. Customize Game       *\n");
    printf("*      3. Exit Game            *\n");
    printf("********************************\n");
    printf("Please select: ");
}

void customize_game(int *board_size, int *win_condition, int *allow_undo, int *timer_enabled, int *time_limit) {
    printf("Enter board size (e.g., 15 for 15x15): ");
    scanf("%d", board_size);
    while(*board_size < 2){
    	printf("Wrong number!!Enter again: ");
    	scanf("%d", board_size);
	}

    printf("Enter number of consecutive pieces to win: ");
    scanf("%d", win_condition);
    while(*win_condition < 2 || *win_condition > *board_size){
    	printf("Wrong number!!Enter again: ");
    	scanf("%d", win_condition);
	}

    printf("Allow undo? (1 for yes, 0 for no): ");
    scanf("%d", allow_undo);
    while(*allow_undo > 1){
    	printf("Wrong number!!Enter again: ");
    	scanf("%d", allow_undo);
	}

    printf("Enable timer? (1 for yes, 0 for no): ");
    scanf("%d", timer_enabled);
    while(*timer_enabled > 1){
    	printf("Wrong number!!Enter again: ");
    	scanf("%d", timer_enabled);
	}

    if (*timer_enabled) {
        printf("Enter time limit per player (in seconds): ");
        scanf("%d", time_limit);
    }
}

void person_person(int board_size, int win_condition, int allow_undo, int timer_enabled, int time_limit) {
    int chessboard[MAX_ROW][MAX_COL] = {BLANK};
    int cursor_x = 0, cursor_y = 0;
    int player = BLACK;
    Move move_history[MAX_ROW * MAX_COL];
    int move_count = 0;
    int black_time = time_limit, white_time = time_limit, black_clock = time_limit, white_clock = time_limit;
    clock_t start_time = clock();
    char key = 0;
    
    draw_chessboard(board_size, board_size, chessboard, cursor_x, cursor_y, black_time, white_time, timer_enabled);

    while (1) {
        if (timer_enabled) {
            int elapsed_time = (clock() - start_time) / CLOCKS_PER_SEC;
            if (player == BLACK) {
                black_time = time_limit - elapsed_time;
                if (black_time == black_clock - 1) {
                	draw_chessboard(board_size, board_size, chessboard, cursor_x, cursor_y, black_time, white_time, timer_enabled);
                	black_clock = black_time;
				}
                if (black_time <= 0) {
                    printf("White wins by timeout!\n");
                    handle_end_game(board_size, win_condition, allow_undo, timer_enabled, time_limit);
                    return;
                }
            } else {
                white_time = time_limit - elapsed_time;
                if (white_time == white_clock - 1) {
                	draw_chessboard(board_size, board_size, chessboard, cursor_x, cursor_y, black_time, white_time, timer_enabled);
                	white_clock = white_time;
				}
                if (white_time <= 0) {
                    printf("Black wins by timeout!\n");
                    handle_end_game(board_size, win_condition, allow_undo, timer_enabled, time_limit);
                    return;
                }
            }
        }

        if (_kbhit()) {
            key = _getch();
            switch (key) {
                case 'w':
                    move_cursor(chessboard, &cursor_x, &cursor_y, 1, board_size);
                    break;
                case 's':
                    move_cursor(chessboard, &cursor_x, &cursor_y, 2, board_size);
                    break;
                case 'a':
                    move_cursor(chessboard, &cursor_x, &cursor_y, 3, board_size);
                    break;
                case 'd':
                    move_cursor(chessboard, &cursor_x, &cursor_y, 4, board_size);
                    break;
                case ' ':
                    if (chessboard[cursor_x][cursor_y] == BLANK) {
                        place_chess(chessboard, cursor_x, cursor_y, player, move_history, &move_count);
                        if (is_win(chessboard, board_size, board_size, win_condition) == player) {
                            printf("%s wins!\n", player == BLACK ? "Black" : "White");
                            handle_end_game(board_size, win_condition, allow_undo, timer_enabled, time_limit);
                            return;
                        }
                        if (is_full(chessboard, board_size, board_size)) {
                            printf("The board is full! It's a draw.\n");
                            handle_end_game(board_size, win_condition, allow_undo, timer_enabled, time_limit);
                            return;
                        }
                        player = (player == BLACK) ? WHITE : BLACK;  
                        start_time = clock();  
                    }
                    break;
                case 'u':
                    if (allow_undo && move_count > 0) {
                        undo_move(chessboard, move_history, &move_count);
                        player = (player == BLACK) ? WHITE : BLACK;
                    }
                    break;
                case 'q': 
                    handle_end_game(board_size, win_condition, allow_undo, timer_enabled, time_limit);
                    return;
                default:
                    break;
            }
            draw_chessboard(board_size, board_size, chessboard, cursor_x, cursor_y, black_time, white_time, timer_enabled);
        }

        Sleep(1); 
    }
}



void handle_end_game(int board_size, int win_condition, int allow_undo, int timer_enabled, int time_limit) {
    int choice;
    end_menu(&choice);
    switch (choice) {
        case 1:
            person_person(board_size, win_condition, allow_undo, timer_enabled, time_limit);
            break;
        case 2:
            exit(0);
            break;
        case 3:
            main();
            break;
    }
}

void end_menu(int *choice) {
    printf("*********************************\n");
    printf("*      1. Play Again            *\n");
    printf("*      2. Exit Game             *\n");
    printf("*      3. Return to Main Menu   *\n");
    printf("*********************************\n");
    do{printf("Please select: ");
        scanf("%d", choice);
        if (*choice < 1 || *choice > 3) {
            printf("Invalid choice! Please enter 1, 2, or 3.\n");
        }
    } while (*choice < 1 || *choice > 3); 
    printf("\033[H\033[J");
}


void move_cursor(int chessboard[][MAX_COL], int *cursor_x, int *cursor_y, int direction, int board_size) {
    switch (direction) {
        case 1: 
            if (*cursor_x > 0) *cursor_x -= 1;
            break;
        case 2: 
            if (*cursor_x < board_size - 1) *cursor_x += 1;
            break;
        case 3: 
            if (*cursor_y > 0) *cursor_y -= 1;
            break;
        case 4:
            if (*cursor_y < board_size - 1) *cursor_y += 1;
            break;
    }
}

void place_chess(int chessboard[][MAX_COL], int cursor_x, int cursor_y, int player, Move *history, int *move_count) {
    chessboard[cursor_x][cursor_y] = player;
    history[*move_count].x = cursor_x;
    history[*move_count].y = cursor_y;
    history[*move_count].player = player;
    (*move_count)++;
}

void undo_move(int chessboard[][MAX_COL], Move *history, int *move_count) {
    if (*move_count > 0) {
        (*move_count)--;
        chessboard[history[*move_count].x][history[*move_count].y] = BLANK;
    }
}

int is_full(int chessboard[][MAX_COL], int row, int col) {
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            if (chessboard[i][j] == BLANK) return 0;
        }
    }
    return 1;
}

int is_win(int chessboard[][MAX_COL], int row, int col, int win_condition) {
    int directions[4][2] = {
        {1, 0},
        {0, 1},
        {1, 1},
        {1, -1}
    };

    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            if (chessboard[i][j] != BLANK) {
                int player = chessboard[i][j];

                for (int d = 0; d < 4; d++) {
                    int consecutive_count = 1;

                    for (int step = 1; step < win_condition; step++) {
                        int new_x = i + step * directions[d][0];
                        int new_y = j + step * directions[d][1];

                        if (new_x >= 0 && new_x < row && new_y >= 0 && new_y < col && chessboard[new_x][new_y] == player) {
                            consecutive_count++;
                        } else {
                            break;
                        }

                        if (consecutive_count == win_condition) {
                            return player;
                        }
                    }
                }
            }
        }
    }
    return 0; 
}
