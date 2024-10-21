#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#define MAX_ROW 15
#define MAX_COL 15
#define WHITE -1
#define BLACK 1
#define BLANK 0

void draw_chessboard(int row, int col, int chessboard[][MAX_COL], int cursor_x, int cursor_y);
void draw_chessman(int x, int y, int type, char *tableline, int cursor_x, int cursor_y);
void draw_menu(void);
void person_person(void);
void person_computer_random(void);
int is_full(int chessboard[][MAX_COL], int row, int col);
int is_win(int chessboard[][MAX_COL], int row, int col);
void move_cursor(int chessboard[][MAX_COL], int *cursor_x, int *cursor_y, int direction);
void place_chess(int chessboard[][MAX_COL], int cursor_x, int cursor_y, int player);
int random_create_point(void);

int main() {
    int choice;
    draw_menu();
    while (1) {
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                person_person();
                break;
            case 2:
                person_computer_random();
                break;
            case 3:
                exit(0);
                break;
            default:
                printf("Error Please Input Again\n");
        }
    }
    return 0;
}

void draw_chessboard(int row, int col, int chessboard[][MAX_COL], int cursor_x, int cursor_y) {
    printf("\033[H\033[J");

    for (int i = 0; i < row; i++) {
        if (i == 0) {
            for (int j = 0; j < col; j++) {
                if (j == 0)
                    draw_chessman(i, j, chessboard[i][j], "┌ ", cursor_x, cursor_y);
                else if (j == 14)
                    draw_chessman(i, j, chessboard[i][j], "┐ ", cursor_x, cursor_y);
                else
                    draw_chessman(i, j, chessboard[i][j], "┬ ", cursor_x, cursor_y);
            }
            printf("\n");
        } else if (i == 14) {
            for (int j = 0; j < col; j++) {
                if (j == 0)
                    draw_chessman(i, j, chessboard[i][j], "└ ", cursor_x, cursor_y);
                else if (j == 14)
                    draw_chessman(i, j, chessboard[i][j], "┘ ", cursor_x, cursor_y);
                else
                    draw_chessman(i, j, chessboard[i][j], "┴ ", cursor_x, cursor_y);
            }
            printf("\n");
        } else {
            for (int j = 0; j < col; j++) {
                if (j == 0)
                    draw_chessman(i, j, chessboard[i][j], "├ ", cursor_x, cursor_y);
                else if (j == 14)
                    draw_chessman(i, j, chessboard[i][j], "┤ ", cursor_x, cursor_y);
                else
                    draw_chessman(i, j, chessboard[i][j], "┼ ", cursor_x, cursor_y);
            }
            printf("\n");
        }
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

int random_create_point(void) {
    return rand() % MAX_ROW;
}

void draw_menu(void) {
    printf("*******************************\n");
    printf("****** Welcome to Gomoku ******\n");
    printf("******     Game mode     ******\n");
    printf("*      1.Play with others     *\n");
    printf("*      2.Play with computer   *\n");
    printf("*      3.Exit                 *\n");
    printf("*******************************\n");
    printf("Please choose :");
}

void person_person(void) {
    int chessboard[MAX_ROW][MAX_COL] = {BLANK};
    int cursor_x = 0, cursor_y = 0;
    int player = BLACK;
    char key;

    draw_chessboard(MAX_ROW, MAX_COL, chessboard, cursor_x, cursor_y);

    while (1) {
        key = _getch();  
        switch (key) {
            case 'w':
                move_cursor(chessboard, &cursor_x, &cursor_y, 1);  // Up
                break;
            case 's':
                move_cursor(chessboard, &cursor_x, &cursor_y, 2);  // Down
                break;
            case 'a':
                move_cursor(chessboard, &cursor_x, &cursor_y, 3);  // Left
                break;
            case 'd':
                move_cursor(chessboard, &cursor_x, &cursor_y, 4);  // Right
                break;
            case ' ':  // Place chess
                if (chessboard[cursor_x][cursor_y] == BLANK) {
                    place_chess(chessboard, cursor_x, cursor_y, player);
                    draw_chessboard(MAX_ROW, MAX_COL, chessboard, cursor_x, cursor_y);
                    if (is_win(chessboard, MAX_ROW, MAX_COL) == player) {
                        printf("%s win!\n", player == BLACK ? "Black" : "White");
                        exit(0);
                    }
                    player = (player == BLACK) ? WHITE : BLACK;
                }
                break;
            default:
                break;
        }

        draw_chessboard(MAX_ROW, MAX_COL, chessboard, cursor_x, cursor_y);

        if (is_full(chessboard, MAX_ROW, MAX_COL)) {
            printf("Chessboard is full\n");
            break;
        }
    }
}

void move_cursor(int chessboard[][MAX_COL], int *cursor_x, int *cursor_y, int direction) {
    switch (direction) {
        case 1:  // Up
            if (*cursor_x > 0) (*cursor_x)--;
            break;
        case 2:  // Down
            if (*cursor_x < MAX_ROW - 1) (*cursor_x)++;
            break;
        case 3:  // Left
            if (*cursor_y > 0) (*cursor_y)--;
            break;
        case 4:  // Right
            if (*cursor_y < MAX_COL - 1) (*cursor_y)++;
            break;
    }
}

void place_chess(int chessboard[][MAX_COL], int cursor_x, int cursor_y, int player) {
    chessboard[cursor_x][cursor_y] = player;
}

int is_full(int chessboard[][MAX_COL], int row, int col) {
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            if (chessboard[i][j] == BLANK)
                return 0;
        }
    }
    return 1;
}

int is_win(int chessboard[][MAX_COL], int row, int col) {
    int i, j;
    for (i = 0; i < row; i++) {
        for (j = 0; j < col; j++) {
            if (chessboard[i][j] == BLANK)
                continue;
            if (j < col - 4)  // Horizontal
                if (chessboard[i][j] == chessboard[i][j + 1] && chessboard[i][j] == chessboard[i][j + 2]
                    && chessboard[i][j] == chessboard[i][j + 3] && chessboard[i][j] == chessboard[i][j + 4])
                    return chessboard[i][j];
            if (i < row - 4)  // Vertical
                if (chessboard[i][j] == chessboard[i + 1][j] && chessboard[i][j] == chessboard[i + 2][j]
                    && chessboard[i][j] == chessboard[i + 3][j] && chessboard[i][j] == chessboard[i + 4][j])
                    return chessboard[i][j];
            if (i < row - 4 && j < col - 4)  // Diagonal 
                if (chessboard[i][j] == chessboard[i + 1][j + 1] && chessboard[i][j] == chessboard[i + 2][j + 2]
                    && chessboard[i][j] == chessboard[i + 3][j + 3] && chessboard[i][j] == chessboard[i + 4][j + 4])
                    return chessboard[i][j];
            if (i < row - 4 && j > 4)  // Diagonal /
                if (chessboard[i][j] == chessboard[i + 1][j - 1] && chessboard[i][j] == chessboard[i + 2][j - 2]
                    && chessboard[i][j] == chessboard[i + 3][j - 3] && chessboard[i][j] == chessboard[i + 4][j - 4])
                    return chessboard[i][j];
        }
    }
    return BLANK;
}

void person_computer_random(void) {
    int chessboard[MAX_ROW][MAX_COL] = {BLANK};
    int cursor_x = 0, cursor_y = 0;
    char key;
    int player = BLACK;
    int a = 1;

    draw_chessboard(MAX_ROW, MAX_COL, chessboard, cursor_x, cursor_y);

    for (int step = 1; step <= MAX_ROW * MAX_COL; step++) {
        if (step % 2 == 1) {  // Player's turn
        a = 1;
            while (a == 1) {
                key = _getch();
                switch (key) {
                    case 'w':
                        move_cursor(chessboard, &cursor_x, &cursor_y, 1);  // Up
                        break;
                    case 's':
                        move_cursor(chessboard, &cursor_x, &cursor_y, 2);  // Down
                        break;
                    case 'a':
                        move_cursor(chessboard, &cursor_x, &cursor_y, 3);  // Left
                        break;
                    case 'd':
                        move_cursor(chessboard, &cursor_x, &cursor_y, 4);  // Right
                        break;
                    case ' ':  // Place chess
                        if (chessboard[cursor_x][cursor_y] == BLANK) {
                            place_chess(chessboard, cursor_x, cursor_y, player);
                            draw_chessboard(MAX_ROW, MAX_COL, chessboard, cursor_x, cursor_y);
                            if (is_win(chessboard, MAX_ROW, MAX_COL) == player) {
                                printf("Black win!\n");
                                exit(0);
                            }
                            a = 2;  // Exit while to move to computer's turn
                        }
                        break;
                    default:
                        break;
                }
                draw_chessboard(MAX_ROW, MAX_COL, chessboard, cursor_x, cursor_y);
            }
        } else {  // Computer's turn
            while (1) {
                int i = random_create_point();
                int j = random_create_point();
                if (chessboard[i][j] == BLANK) {
                    chessboard[i][j] = WHITE;
                    draw_chessboard(MAX_ROW, MAX_COL, chessboard, cursor_x, cursor_y);
                    if (is_win(chessboard, MAX_ROW, MAX_COL) == WHITE) {
                        printf("White win!\n");
                        exit(0);
                    }
                    break;
                }
            }
        }

        if (is_full(chessboard, MAX_ROW, MAX_COL)) {
            printf("Chessboard is full\n");
            break;
        }
    }
}

