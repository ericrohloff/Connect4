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
void disp_board(int board[ROWS][COLS])
{
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            printf("%d ", board[i][j]);
        }
        printf("\n");
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

    adjacent = 0;
    while (colChecker <= colMax)
    {
        if (board[row][colChecker] == player)
        {
            adjacent++;
            if (adjacent == 4)
            {
                return player;
            }
        }
        else
        {
            adjacent = 0;
        }
        colChecker++;
    }

    // Check \ Diagonal
    int minLeft = 3;
    int maxRight = 3;
    if (row < col)
    {
        if (row - minLeft < 0)
        {
            minLeft = row;
        }
    }
    else
    {
        if (col - minLeft < 0)
        {
            minLeft = col;
        }
    }
    if (ROWS - row < COLS - col)
    {
        if (row + maxRight > ROWS - 1)
        {
            maxRight = ROWS - row - 1;
        }
    }
    else
    {
        if (col + maxRight > COLS - 1)
        {
            maxRight = COLS - col - 1;
        }
    }

    rowChecker = row - minLeft;
    colChecker = col - minLeft;
    rowMax = row + maxRight;
    adjacent = 0;

    while (rowChecker <= rowMax)
    {
        if (board[rowChecker][colChecker] == player)
        {
            adjacent++;
            if (adjacent == 4)
            {
                return player;
            }
        }
        else
        {
            adjacent = 0;
        }
        rowChecker++;
        colChecker++;
    }

    // Check / Diagonal
    minLeft = 3;
    maxRight = 3;
    if (ROWS - row - 1 < col)
    {
        if (minLeft + row > ROWS - 1)
        {
            minLeft = ROWS - row - 1;
        }
    }
    else
    {
        if (col - minLeft < 0)
        {
            minLeft = col;
        }
    }
    if (row < COLS - col - 1)
    {
        if (row - maxRight < 0)
        {
            maxRight = row;
        }
    }
    else
    {
        if (maxRight + col > COLS - 1)
        {
            maxRight = COLS - col - 1;
        }
    }

    rowChecker = row + minLeft;
    colChecker = col - minLeft;
    rowMax = row - maxRight;

    adjacent = 0;
    while (rowChecker >= rowMax)
    {
        if (board[rowChecker][colChecker] == player)
        {
            adjacent++;
            if (adjacent == 4)
            {
                return player;
            }
        }
        else
        {
            adjacent = 0;
        }
        rowChecker--;
        colChecker++;
    }
}

// Method to place the players piece onto the board
int set_piece(int col, int player, int board[ROWS][COLS])
{
    int lowRow = ROWS - 1;

    // Find the first available row in the column
    while (lowRow >= 0 && board[lowRow][col] != 0)
    {
        lowRow--;
    }
    if (lowRow >= 0)
    {
        board[lowRow][col] = player;
    }
    else
    {
        // Column is full, handle this case appropriately (e.g., notify the user)
        printf("Column %d is full. Try a different column.\n", col);
    }

    return check_win(lowRow, col, board);
}

int main()
{
    init_board();
    printf("%d", set_piece(3, 1, board));
    printf("\n");
    printf("%d", set_piece(2, 2, board));
    printf("\n");
    printf("%d", set_piece(2, 1, board));
    printf("\n");
    printf("%d", set_piece(1, 2, board));
    printf("\n");
    printf("%d", set_piece(1, 2, board));
    printf("\n");
    printf("%d", set_piece(1, 1, board));
    printf("\n");
    printf("%d", set_piece(0, 2, board));
    printf("\n");
    printf("%d", set_piece(0, 2, board));
    printf("\n");
    printf("%d", set_piece(0, 2, board));
    printf("\n");
    printf("%d", set_piece(0, 1, board));
    printf("\n");

    // printf(set_piece(1, 1, board));
    // printf(set_piece(0, 1, board));
    // printf(set_piece(0, 1, board));
    // printf(set_piece(0, 1, board));
    disp_board(board);
}
