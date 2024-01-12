#ifndef LIGUE4
#define LIGUE4

// the ligue4 project header

// including an unreasonable
// amount of libraries:
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <ncurses.h>
#include <assert.h>

// constants
#define HEIGHT     3
#define WIDTH      5
#define NB_LINES   6
#define NB_COLS    7

#define NO_PLAYER  0
#define PLAYER_1   1
#define PLAYER_2   2
#define BOARD      3

#define COLOR_PLAYER_1  COLOR_CYAN
#define COLOR_PLAYER_2  COLOR_MAGENTA
#define COLOR_BOARD     COLOR_WHITE


struct game_state {
	unsigned char *board;
	unsigned char turn;
	unsigned char winner;
	int candidate_pos;
};

// function declarations
void display_disc(int startx, int starty, int posx, int posy, unsigned char player);
void display_discs(int startx, int starty, unsigned char *board);
void display_board(int startx, int starty);
void display_candidate(int startx, int starty, int pos, unsigned char player);
void display_instructions();
bool insert_disc(unsigned char *board, int pos, unsigned char player);
unsigned char check_4_inrow(unsigned char *board, int startx, int starty, int dirx, int diry);
unsigned char check_victory(unsigned char *board);
void init_game(struct game_state *state);
void update(struct game_state *state, int ch);
void redraw(struct game_state *state);
void sig_handler(int sig);

#endif
