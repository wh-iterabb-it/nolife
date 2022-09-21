/*
    game of life: wh.iterabb.it edition
*/

#include <locale.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h> // in this house we are POSIX only
#include <wchar.h>

// Constants
#define ROWS 25
#define COLS 25
#define LIVE 1
#define DEAD 0
#define BUNNY L"🐇"

void clear_terminal(void);
void display_board(int game_board[ROWS][COLS]);
void generate(int game_board[ROWS][COLS], bool *playing);
bool is_empty(int game_board[ROWS][COLS]);
int neighborhood(int game_board[ROWS][COLS], int row, int col);

int main(void) {

    setlocale(LC_ALL, ""); /* TODO: Error checking, this isn't guaranteed to
                            work on all systems. */

    int game_board[ROWS][COLS] = { {0,0} };
    bool playing = true;

    /* glider */
    game_board[0][1] = LIVE;
    game_board[1][2] = LIVE;
    game_board[2][0] = LIVE;
    game_board[2][1] = LIVE;
    game_board[2][2] = LIVE;

    clear_terminal();

    while (playing) {
        display_board(game_board);
        sleep(1);
        generate(game_board, &playing);
        clear_terminal();
    }

    return 0;
}

void generate(int game_board[ROWS][COLS], bool *playing) {
    int temp_board[ROWS][COLS] = {0};

    if (is_empty(game_board) == true)
        *playing = false;

    for (int row = 0; row < ROWS; ++row) {
        for (int col = 0; col < COLS; ++col) {
            int neighbors = neighborhood(game_board, row, col);

            if (game_board[row][col] == LIVE) {
                if (neighbors < 2) {
                    temp_board[row][col] = DEAD;
                }
                if (neighbors >= 2 && neighbors <= 3)
                    temp_board[row][col] = LIVE;
                if (neighbors > 3)
                    temp_board[row][col] = DEAD;
            } else {
                if (neighbors == 3)
                    temp_board[row][col] = LIVE;
            }
        }
    }

    memcpy(game_board, temp_board, sizeof(game_board[0]) * COLS);
    // https://stackoverflow.com/questions/69329884/is-copying-2d-arrays-with-memcpy-technically-undefined-behaviour?noredirect=1&lq=1
}

bool is_empty(int game_board[ROWS][COLS]) {
    bool empty = true;

    for (int *p = &game_board[0][0]; p <= &game_board[ROWS - 1][COLS - 1]; ++p) {
        if (*p == LIVE)
            empty = false;
    }

    return empty;
}

int neighborhood(int game_board[ROWS][COLS], int row, int col) {
    int neighbors = 0;

    if (row != 0)
        neighbors += game_board[row-1][col];   // up
    if (row != ROWS)
        neighbors += game_board[row+1][col];   // down
    if (col != 0)
        neighbors += game_board[row][col-1];   // left
    if (col != COLS)
        neighbors += game_board[row][col+1];   // right
    if (row != 0 && col != 0)
        neighbors += game_board[row-1][col-1]; // up-left
    if (row != 0 && col != COLS)
        neighbors += game_board[row-1][col+1]; // up-right
    if (row != ROWS && col != 0)
        neighbors += game_board[row+1][col-1]; // down-left
    if (row != ROWS && col != COLS)
        neighbors += game_board[row+1][col+1]; // down-right

    return neighbors;
}

void display_board(int game_board[ROWS][COLS]) {
    /*
        print the board using pointer arithmetic instead of nested loops. At
        some point on certain compilers this might've have been faster, but
        there's probably no good reason to do this today.
    */

    int *p, i = 0;
    for (p = &game_board[0][0]; p <= &game_board[ROWS - 1][COLS - 1]; ++p, ++i) {
        if (*p == LIVE) {
            wprintf(L"%ls", BUNNY);
            fflush(stdout);
        } else {
            printf("- ");
            fflush(stdout);
        }

        /* figure out where to print newline, because you can't easily use mod
        for pointer arithmetic :) */
        if ((i + 1) % COLS == 0)
            putchar('\n');
    }
}

void clear_terminal(void) {
    /* flush the buffer and clear the screen (again POSIX only) */
    fflush(stdout);
    system("clear");
}