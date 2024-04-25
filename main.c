#include "stm32l476xx.h"
#include "SysClock.h"
#include "LED.h"
#include "UART.h"
#include "audio.h"

#include <string.h>
#include <stdio.h>

#include "SysTick.h"

uint8_t buffer[2048];

struct song left_right = {1, {{200, 8.8, 5}}};
struct song full = {2, {{200, 9.8, 5}, {500, 8.8, 5}}};
struct song place = {2, {{200, 7.5, 5}, {500, 10.8, 5}}};
struct song win_song = {3, {{550, 8.6, 3}, {200, 6.6, 3}, {550, 10.3, 3}}};

// Authors: Eric Rohloff, Josh Wilkie, Abe Nelson
// CONNECT4 STM32L4
// Game logic and functionality to display to LCD

#include <stdio.h>
#include <stdbool.h>

#define ROWS 6
#define COLS 7

#define P1 "\x1b[31m\U000025CF\x1b[37m"
#define P2 "\x1b[33m\U000025CF\x1b[37m"

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
	int n = sprintf((char *) buffer, "\x1b[2J\x1b[31m----------------------------------------\r\n"); 
	n += sprintf((char *) buffer + n, ":\x1b[33m  ____                            _   \x1b[31m:\r\n"); 
	n += sprintf((char *) buffer + n, ":\x1b[33m / ___|___  _ __  _ __   ___  ___| |_ \x1b[31m:\r\n"); 
	n += sprintf((char *) buffer + n, ":\x1b[33m| |   / _ \\| '_ \\| '_ \\ / _ \\/ __| __|\x1b[31m:\r\n"); 
	n += sprintf((char *) buffer + n, ":\x1b[33m| |__| (_) | | | | | | |  __/ (__| |_ \x1b[31m:\r\n"); 
	n += sprintf((char *) buffer + n, ":\x1b[33m \\____\\___/|_| |_|_|_|_|\\___|\\___|\\__|\x1b[31m:\r\n"); 
	n += sprintf((char *) buffer + n, ":                \x1b[33m| || |\x1b[31m                :\r\n"); 
	n += sprintf((char *) buffer + n, ":                \x1b[33m| || |_\x1b[31m               :\r\n"); 
	n += sprintf((char *) buffer + n, ":                \x1b[33m|__   _|\x1b[31m              :\r\n"); 
	n += sprintf((char *) buffer + n, ":                   \x1b[33m|_|\x1b[31m                :\r\n"); 
	n += sprintf((char *) buffer + n, "----------------------------------------\r\n"); 
	n += sprintf((char *) buffer + n, "\x1b[34mCreated by: Eric Rohloff, Josh Wilkie, Abe Nelson\r\n\x1b[37m");
	n += sprintf((char *) buffer + n, "\r\n");
	n += sprintf((char *) buffer + n, "\r\n");

	USART_Write(USART2, buffer, n);
}

// Method to display baord onto the LCD screen
void disp_board(int col, int board[ROWS][COLS], int turn)
{
		int n;
		n = sprintf((char*)buffer, "\x1b[2J");
		if (turn == 1) {
			n += sprintf((char *)buffer + n, "Player %s, press B to place a chip: \r\n", P1);
		} else {
			n += sprintf((char *)buffer + n, "Player %s, press B to place a chip: \r\n", P2);
		}
		
		USART_Write(USART2, buffer, n);
	
		for (int i = 0; i <= 6; i++) {
			if (i != col) n += sprintf((char *) buffer + n, "  %d ", i);
			else if (i == col && turn == 1) n += sprintf((char *) buffer + n, "  %s ", P1);
			else if (i == col && turn == 2) n += sprintf((char *) buffer + n, "  %s ", P2);
		}
    n += sprintf((char*)buffer + n, "\r\n");
    n += sprintf((char *)buffer + n, "\x1b[34m----+---+---+---+---+---+---+\r\n");
		USART_Write(USART2, buffer, n);
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            // Move cursor to specific location for each cell
						n = sprintf((char *) buffer, "\x1b[34m|");
            if (board[i][j] == 0)
                n += sprintf((char *) buffer + n, "   ");
            else if (board[i][j] == 1)
                n += sprintf((char *) buffer + n, "\x1b[31m \U000025CF \x1b[37m");
            else if (board[i][j] == 2)
                n += sprintf((char *) buffer + n, "\x1b[33m \U000025CF \x1b[37m");
						
						USART_Write(USART2, buffer, n);
        }
        n = sprintf((char*) buffer, "\x1b[34m|\r\n");
				USART_Write(USART2, buffer, n);
    }
    n = sprintf((char *) buffer, "+---+---+---+---+---+---+---+\x1b[37m\r\n");
		USART_Write(USART2, buffer, n);
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
				playsong(place);
    }
    else
    {
      // Column is full, handle this case appropriately (e.g., notify the user)
      playsong(full);
			return -1;
    }

    return check_win(lowRow, col, board);
}

#define joystick_center 0x1
#define joystick_left 0x2
#define joystick_right 0x4

extern volatile uint8_t nes_controls[8];
#define NES_A 1
#define NES_LEFT 6
#define NES_RIGHT 7


void wait_for_start_btn(void) {
	while (nes_controls[3] > 0);
}

int get_column(int col, int turn) {
	while (1) {
		if(nes_controls[NES_A] == 0) {
			while(nes_controls[NES_A] == 0);
			return col;
		} else if(nes_controls[NES_LEFT] == 0){
			col--;
			if (col < 0) col = 0;
			disp_board(col, board, turn);
			while(nes_controls[NES_LEFT] == 0);
			//playsong(left_right);
		} else if(nes_controls[NES_RIGHT] == 0){
			col++;
			if (col > 6) col = 6;
			disp_board(col, board, turn);
			while(nes_controls[NES_RIGHT] == 0);
			//playsong(left_right);
		}
	}
	
}


#define NES_latch 0x2;
#define NES_clk 0x4;
#define NES_data 0x20;



void read_NES(void) {
	// pulse latch
	GPIOB->ODR |= NES_latch;
	for (int i = 0; i < 1000000; i++) {}
	GPIOB->ODR &= ~NES_latch;
		for (int i = 0; i < 1000000; i++) {}
}

void Joystick_Initialization(void){
	// Enable clock for GPIOA, then configure appropriate pins PA0, PA1, and PA2 for joystick
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	GPIOA->MODER &= ~(0x3F);
	GPIOA->PUPDR &= ~(0x3F);
	GPIOA->PUPDR |= 0x2A;
	
}

void NES_Init(void) {
	// PINS
	// clock (out): pb3
	// latch (out): pb2
	// data   (in): pb6
	// 0000 0000 0101 0000
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
	
	GPIOB->MODER &= ~(3U<<(2*2));  
	GPIOB->MODER |= 1U<<(2*2);
	
	GPIOB->MODER &= ~(3U<<(3*2));  
	GPIOB->MODER |= 1U<<(3*2);
	
	GPIOB->MODER &= ~(3U<<(6*2));
	
	// GPIO Push-Pull: No pull-up, pull-down (00), Pull-up (01), Pull-down (10), Reserved (11)
	GPIOB->PUPDR &= ~(3U<<(3*2));  // No pull-up, no pull-down
}

void reset_game(int *turn, int *col) {
		// Initialize the game board
  init_board();
  draw_logo();
  //disp_board(board);
	
	int n;
  
	
  // Check win condition
	
	n = sprintf((char *) buffer, "Player 1 is \x1b[31m\U000025CF\x1b[37m, Player 2 is \x1b[33m\U000025CF\x1b[37m\r\n\r\n");
	n += sprintf((char *) buffer + n, "Press \x1b[30;41mSTART\x1b[37;40m to play\r\n");
	USART_Write(USART2, buffer, n);
	
	wait_for_start_btn();
	
	disp_board(3, board, 1);
	
	*turn = 1;
	*col = 3;
}

int main()
{
	System_Clock_Init();
	Joystick_Initialization();
	UART2_Init();
	LED_Init();
	NES_Init();
	SysTick_Init(999);
	
	DAC_Init();
	TIM4_Init();
	
	int turn;
	int col;
	
	int n = 0;
	
	reset_game(&turn, &col);
		
	while (1)
	{

		  
		
			col = get_column(col, turn);
			playsong(left_right);
		
//			n = sprintf((char *) buffer, "Player 1 is X, Player 2 is O\r\n");
//			USART_Write(USART2, buffer, n);
		
			int win = set_piece(col, turn, board);
				

			if (turn == 1 && win != -1)
			{
					turn = 2;
			}
			else if (win != -1)
			{
					turn = 1;
			}
			
			disp_board(col, board, turn);
			if (win < 0) {
				n = sprintf((char *) buffer, "Column %d is full. Try a different column.\r\n", col);
				USART_Write(USART2, buffer, n);
			}
			
			if (win > 0) {
				if (win == 1)
				{
						n = sprintf((char *)buffer, "Player %s wins!\r\n", P1);
						USART_Write(USART2, buffer, n);
				}
				else if (win == 2)
				{
						n = sprintf((char *)buffer, "Player %s wins!\r\n", P2);
						USART_Write(USART2, buffer, n);
				}
				
				n = sprintf((char *) buffer, "Press START to play again\r\n");
				USART_Write(USART2, buffer, n);
				playsong(win_song);
				wait_for_start_btn();
				
				reset_game(&turn, &col);

			}
			
	}
	return 0;
}
