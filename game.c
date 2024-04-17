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

int check_win(int row, int col)
{
    int player = board[row][col];

    // Check horizontal
    int count = 1;

    // Check left
    for (int i = col - 1; i >= 0 && board[row][i] == player; i--)
    {
        count++;
    }

    // Check right
    for (int i = col + 1; i < COLS && board[row][i] == player; i++)
    {
        count++;
    }

    if (count >= 4)
    {
        return player;
    }

    // Check vertical
    count = 1;

    // Check downward
    for (int i = row + 1; i < ROWS && board[i][col] == player; i++)
    {
        count++;
    }

    if (count >= 4)
    {
        return player;
    }

    // Check diagonal from top-left to bottom-right
    count = 1;

    // Check top-left
    for (int i = row - 1, j = col - 1; i >= 0 && j >= 0 && board[i][j] == player; i--, j--)
    {
        count++;
    }

    // Check bottom-right
    for (int i = row + 1, j = col + 1; i < ROWS && j < COLS && board[i][j] == player; i++, j++)
    {
        count++;
    }

    if (count >= 4)
    {
        return player;
    }

    count = 1;

    // Check top-right
    for (int i = row - 1, j = col + 1; i >= 0 && j < COLS && board[i][j] == player; i--, j++)
    {
        count++;
    }

    // Check bottom-left
    for (int i = row + 1, j = col - 1; i < ROWS && j >= 0 && board[i][j] == player; i++, j--)
    {
        count++;
    }

    if (count >= 4)
    {
        return player;
    }

    return 0;
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

    return check_win(lowRow, col);
}

int main()
{
    init_board();
    printf("%d", set_piece(0, 1, board));
    printf("\n");
    printf("%d", set_piece(1, 2, board));
    printf("\n");
    printf("%d", set_piece(1, 1, board));
    printf("\n");
    printf("%d", set_piece(2, 2, board));
    printf("\n");
    printf("%d", set_piece(2, 2, board));
    printf("\n");
    printf("%d", set_piece(2, 1, board));
    printf("\n");
    printf("%d", set_piece(3, 2, board));
    printf("\n");
    printf("%d", set_piece(3, 2, board));
    printf("\n");
    printf("%d", set_piece(3, 2, board));
    printf("\n");
    printf("%d", set_piece(3, 1, board));
    printf("\n");
    disp_board(board);
}
