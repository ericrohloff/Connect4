// Authors: Eric Rohloff, Josh Wilkie, Abe Nelson
// CONNECT4 STM32L4
// Game logic and functionality to display to LCD

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
    while (lowRow >= 0 && board[lowRow][col] == 0)
    {
        lowRow--;
    }
    if (lowRow >= 0)
    {
        board[lowRow][col] = player;
    }
    else
    {
        // This will happen if the column is full, will display to the STM32L4
        // display, with color and sound effect.
    }
}

int check_win(int row, int col, int board[ROWS][COLS])
{
    int player = board[row][col];
    int rowChecker = row;
    int colChecker = col;
    int adjacent = 0;
    int colMax = 6;
    int rowMax = 6;

    // Check Vertical
    if (row <= 2)
    {
        while (rowChecker <= 5)
        {
            if (board[rowChecker][col] == player)
            {
                adjacent++;
                rowChecker++;
                if (adjacent == 4)
                {
                    return player;
                }
            }
            else
            {
                rowChecker = row;
                adjacent = 0;
                break;
            }
        }
    }

    // Check Horizontal
    if (col > 2)
    {
        colChecker = col - 3;
        colMax = 6;
    }
    else
    {
        colChecker = 0;
        colMax = col + 3;
    }

    while (colChecker < colMax)
    {
        if (board[row][colChecker] == player)
        {
            adjacent++;
            colChecker++;
            if (adjacent == 4)
            {
                return player;
            }
        }
        else
        {
            adjacent = 0;
        }
    }

    // Check \ Diagonal
    if (col > row)
    {
        if (row > 2)
        {
            rowChecker = row - 3;
            colChecker = col - 3;
        }
        else
        {
            rowMax = 0;
            colMax = col - row;
        }
    }

    // Check / Diagonal
}
