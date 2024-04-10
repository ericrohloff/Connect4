#include <stdio.h>
#include <stdbool.h>

#define ROWS 6
#define COLS 7

// Code for creating the framework for Connect4

int board[ROWS][COLS];

// Method to initialize the board to zero in order to then add characters
void init_board()
{
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            board[i][j] = 0;
        }
    }
}

// Method to display baord onto the LCD screen
void disp_board()
{
    // TODO: Implement this
}

// Method to place the players piece onto the board
void set_piece(int col, int player, int board[ROWS][COLS])
{
    int lowRow = ROWS - 1;
    while (lowRow >= 0 && board[lowRow][col] = 0)
    {
        lowRow--;
    }
}
