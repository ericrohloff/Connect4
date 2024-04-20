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

void draw_logo()
{

    printf("        CCCCCCCCCCCCC     OOOOOOOOO                                                                                          tttt                  ffffffffffffffff\n");
    printf("     CCC::::::::::::C   OO:::::::::OO                                                                                     ttt:::t                 f::::::::::::::::f\n");
    printf("   CC:::::::::::::::C OO:::::::::::::OO                                                                                   t:::::t                f::::::::::::::::::f\n");
    printf("  C:::::CCCCCCCC::::CO:::::::OOO:::::::O                                                                                  t:::::t                f::::::fffffff:::::f\n");
    printf(" C:::::C       CCCCCCO::::::O   O::::::Onnnn  nnnnnnnn    nnnn  nnnnnnnn        eeeeeeeeeeee        ccccccccccccccccttttttt:::::ttttttt          f:::::f       ffffffooooooooooo   uuuuuu    uuuuuu rrrrr   rrrrrrrrr\n");
    printf("C:::::C              O:::::O     O:::::On:::nn::::::::nn  n:::nn::::::::nn    ee::::::::::::ee    cc:::::::::::::::ct:::::::::::::::::t          f:::::f           oo:::::::::::oo u::::u    u::::u r::::rrr:::::::::r\n");
    printf("C:::::C              O:::::O     O:::::On::::::::::::::nn n::::::::::::::nn  e::::::eeeee:::::ee c:::::::::::::::::ct:::::::::::::::::t         f:::::::ffffff    o:::::::::::::::ou::::u    u::::u r:::::::::::::::::r\n");
    printf("C:::::C              O:::::O     O:::::Onn:::::::::::::::nnn:::::::::::::::ne::::::e     e:::::ec:::::::cccccc:::::ctttttt:::::::tttttt         f::::::::::::f    o:::::ooooo:::::ou::::u    u::::u rr::::::rrrrr::::::r\n");
    printf("C:::::C              O:::::O     O:::::O  n:::::nnnn:::::n  n:::::nnnn:::::ne:::::::eeeee::::::ec::::::c     ccccccc      t:::::t               f::::::::::::f    o::::o     o::::ou::::u    u::::u  r:::::r     r:::::r\n");
    printf("C:::::C              O:::::O     O:::::O  n::::n    n::::n  n::::n    n::::ne:::::::::::::::::e c:::::c                   t:::::t               f:::::::ffffff    o::::o     o::::ou::::u    u::::u  r:::::r     rrrrrrr\n");
    printf("C:::::C              O:::::O     O:::::O  n::::n    n::::n  n::::n    n::::ne::::::eeeeeeeeeee  c:::::c                   t:::::t                f:::::f          o::::o     o::::ou::::u    u::::u  r:::::r\n");
    printf(" C:::::C       CCCCCCO::::::O   O::::::O  n::::n    n::::n  n::::n    n::::ne:::::::e           c::::::c     ccccccc      t:::::t    tttttt      f:::::f          o::::o     o::::ou:::::uuuu:::::u  r:::::r\n");
    printf("  C:::::CCCCCCCC::::CO:::::::OOO:::::::O  n::::n    n::::n  n::::n    n::::ne::::::::e          c:::::::cccccc:::::c      t::::::tttt:::::t     f:::::::f         o:::::ooooo:::::ou:::::::::::::::uur:::::r\n");
    printf("   CC:::::::::::::::C OO:::::::::::::OO   n::::n    n::::n  n::::n    n::::n e::::::::eeeeeeee   c:::::::::::::::::c      tt::::::::::::::t     f:::::::f         o:::::::::::::::o u:::::::::::::::ur:::::r\n");
    printf("    CCC::::::::::::C   OO:::::::::OO     n::::n    n::::n  n::::n    n::::n  ee:::::::::::::e    cc:::::::::::::::c        tt:::::::::::tt     f:::::::f          oo:::::::::::oo   uu::::::::uu:::ur:::::r\n");
    printf("       CCCCCCCCCCCCC     OOOOOOOOO       nnnnnn    nnnnnn  nnnnnn    nnnnnn    eeeeeeeeeeeeee      cccccccccccccccc          ttttttttttt       fffffffff            ooooooooooo       uuuuuuuu  uuuurrrrrrr\n");
    printf("\n");
    printf("Created by: Eric Rohloff, Josh Wilkie, Abe Nelson\n");
    printf("\n");
    printf("\n");
}

// Method to display baord onto the LCD screen
void disp_board(int board[ROWS][COLS])
{
    printf("  0   1   2   3   4   5   6\n");
    printf("+---+---+---+---+---+---+---+\n");
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            // Move cursor to specific location for each cell
            printf("|");
            if (board[i][j] == 0)
                printf("   ");
            else if (board[i][j] == 1)
                printf(" X ");
            else if (board[i][j] == 2)
                printf(" O ");
        }
        printf("|\n");
    }
    printf("+---+---+---+---+---+---+---+\n");
}

int check_win(int row, int col, int board[ROWS][COLS])
{
    int player = board[row][col];
    int rowChecker = row;
    int colChecker = col;
    int adjacent = 0;
    int colMax = 6;
    int rowMax = 6;
    int distToLeft = col;
    int distToRight = COLS - col - 1;
    int distToTop = row;
    int distToBot = ROWS - row - 1;

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
    int leftOffset = 3;
    int rightOffset = 3;
    int closest = 0;
    if (distToLeft < distToTop)
    {
        closest = distToLeft;
    }
    else
    {
        closest = distToTop;
    }
    if (closest > 3)
    {
        leftOffset = 3;
    }
    else
    {
        leftOffset = closest;
    }
    if (distToRight < distToBot)
    {
        closest = distToRight;
    }
    else
    {
        closest = distToBot;
    }
    if (closest > 3)
    {
        rightOffset = 3;
    }
    else
    {
        rightOffset = closest;
    }
    rowChecker = row - leftOffset;
    colChecker = col - leftOffset;
    rowMax = row + rightOffset;
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
    if (distToLeft < distToBot)
    {
        closest = distToLeft;
    }
    else
    {
        closest = distToBot;
    }
    if (closest > 3)
    {
        leftOffset = 3;
    }
    else
    {
        leftOffset = closest;
    }
    if (distToRight < distToTop)
    {
        closest = distToRight;
    }
    else
    {
        closest = distToTop;
    }
    if (closest > 3)
    {
        rightOffset = 3;
    }
    else
    {
        rightOffset = closest;
    }
    rowChecker = row + leftOffset;
    colChecker = col - leftOffset;
    rowMax = row - rightOffset;
    adjacent = 0;

    while (rowChecker >= rowMax)
    {
        // printf("%d", board[rowChecker][colChecker]);
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

    return check_win(lowRow, col, board);
}

int main()
{
    // Initialize the game board
    init_board();
    draw_logo();
    disp_board(board);
    printf("\n");
    printf("Player 1 is X, Player 2 is O\n");
    char turn = 1;
    // Check win condition
    while (1)
    {
        int col;
        printf("Player %d, enter a column: ", turn);
        scanf("%d", &col);
        int win = set_piece(col, turn, board);
        disp_board(board);
        if (win == 1)
        {
            printf("Player 1 wins!\n");
            break;
        }
        else if (win == 2)
        {
            printf("Player 2 wins!\n");
            break;
        }
        if (turn == 1)
        {
            turn = 2;
        }
        else
        {
            turn = 1;
        }
    }
    return 0;
}