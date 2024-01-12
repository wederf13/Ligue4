#include "../include/ligue4.h"

void display_disc(int startx, int starty, int posx, int posy, unsigned char player) {
  int x = posx * WIDTH + 1;
	int y = posy * HEIGHT + 1;

	attron(COLOR_PAIR(player));

	mvaddch(starty + y, startx + x, ACS_BOARD);
	mvaddch(starty + y, startx + x + 1, ACS_BOARD);
	mvaddch(starty + y, startx + x + 2, ACS_BOARD);
	mvaddch(starty + y, startx + x + 3, ACS_BOARD);

	mvaddch(starty + y + 1, startx + x, ACS_BOARD);
	mvaddch(starty + y + 1, startx + x + 1, ACS_BOARD);
	mvaddch(starty + y + 1, startx + x + 2, ACS_BOARD);
	mvaddch(starty + y + 1, startx + x + 3, ACS_BOARD);

	attroff(COLOR_PAIR(player));
}

void display_discs(int startx, int starty, unsigned char *board) {
  for (int i = 0; i < NB_LINES; i++) {
    for (int j = 0; j < NB_COLS; j++) {
      unsigned char player = board[i * NB_COLS + j];
      if (player != NO_PLAYER)
        display_disc(startx, starty, j, i, player);
    }
  }
}

void display_board(int startx, int starty) {
  int endy = starty + NB_LINES * HEIGHT;
  int endx = startx + NB_COLS * WIDTH;

  attron(COLOR_PAIR(BOARD));

  // draw grid
	for (int i = starty; i <= endy; i += HEIGHT)
		for (int j = startx; j <= endx; j++)
			mvaddch(i, j, ACS_HLINE);

	for (int i = startx; i <= endx; i += WIDTH)
		for (int j = starty; j <= endy; j++)
			mvaddch(j, i, ACS_VLINE);

	// draw joints
	for (int i = startx + WIDTH; i <= endx - WIDTH; i += WIDTH) {
		mvaddch(starty, i, ACS_TTEE);
		mvaddch(endy, i, ACS_BTEE);
	}

	for (int i = starty + HEIGHT; i <= endy - HEIGHT; i += HEIGHT) {
		mvaddch(i, startx, ACS_LTEE);
		mvaddch(i, endx, ACS_RTEE);
	}

	// draw corners
	mvaddch(starty, startx, ACS_ULCORNER);
	mvaddch(starty, endx, ACS_URCORNER);
	mvaddch(endy, startx, ACS_LLCORNER);
	mvaddch(endy, endx, ACS_LRCORNER);

	attroff(COLOR_PAIR(BOARD));

}

void display_candidate(int startx, int starty, int pos, unsigned char player) {
  display_disc(startx, starty, pos, -1, player);
}

void display_instructions() {
  mvprintw(0, 1, "Player 1: xx  Player 2: xx");
	mvprintw(2, 1, "LEFT:   Esquerda");
	mvprintw(3, 1, "RIGHT:  Direita");
	mvprintw(4, 1, "DOWN:   Colocar peça");
	mvprintw(5, 1, "SPACE:  Reiniciar Jogo");
	mvprintw(6, 1, "CTRL + C: Sair");
	attron(COLOR_PAIR(PLAYER_1));
	mvaddch(0, 11, ACS_BOARD);
	mvaddch(0, 12, ACS_BOARD);
	attron(COLOR_PAIR(PLAYER_2));
	mvaddch(0, 25, ACS_BOARD);
	mvaddch(0, 26, ACS_BOARD);
	attroff(COLOR_PAIR(PLAYER_2));
}

bool insert_disc(unsigned char *board, int pos, unsigned char player) {
  for (int i = 5; i >= 0; i--) {
    if (board[i * NB_COLS + pos] == 0) {
      board[i * NB_COLS + pos] = player;
      return true;
    }
  }
  return false;
}

unsigned char check_4_inrow(unsigned char *board, int startx, int starty, int dirx, int diry) {
  
  unsigned char player = board[starty * NB_COLS + startx];

  for (int i = 1; i < 4; i++) {
    if (startx + i * dirx >= NB_COLS || startx + i * dirx < 0)
			return NO_PLAYER;
		else if (starty + i * diry >= NB_LINES || starty + i * diry < 0)
			return NO_PLAYER;
		else if (board[(starty + i * diry) * NB_COLS + (startx + i * dirx)] != player)
			return NO_PLAYER;
  }
  return player;
}

unsigned char check_victory(unsigned char *board) {
  unsigned char player = NO_PLAYER;

	for (int y = 0; y < NB_LINES; y++)
		for (int x = 0; x < NB_COLS; x++) {
			if (board[y * NB_COLS + x] == NO_PLAYER)
				continue;

			player =  check_4_inrow(board, x, y, 1, 0);   // Horizontal
			player += check_4_inrow(board, x, y, 0, 1);   // Vertical
			player += check_4_inrow(board, x, y, 1, 1);   // Diagonal 1
			player += check_4_inrow(board, x, y, 1, -1);  // Diagonal 2

			if (player != NO_PLAYER)
				return player;
		}

	return NO_PLAYER;
}
void init_game(struct game_state *state) {
  if (state->board == NULL)
		state->board = calloc(NB_LINES * NB_COLS, sizeof(unsigned char));
	else
		memset(state->board, 0, NB_LINES * NB_COLS);

	assert(state->board != NULL);

	state->turn = PLAYER_1;
	state->winner = NO_PLAYER;
	state->candidate_pos = 0;
}

void update(struct game_state *state, int ch) {
  if (ch == KEY_LEFT && state->candidate_pos > 0) {
		state->candidate_pos--;
	}
	else if (ch == KEY_RIGHT && state->candidate_pos < 6) {
		state->candidate_pos++;
	}
	else if (state->winner == NO_PLAYER && ch == KEY_DOWN) {
		// fill board
		int result = insert_disc(state->board, state->candidate_pos, state->turn);

		// change player
		if (result == true)
			state->turn = (state->turn == PLAYER_1) ? PLAYER_2 : PLAYER_1;
	}
	else if (ch == ' ') {
		init_game(state);
	}

	// check victory
	unsigned char winner = check_victory(state->board);
	if (winner != NO_PLAYER) {
		state->winner = winner;
	}
}
void redraw(struct game_state *state) {
  int starty = (LINES - NB_LINES * HEIGHT) / 2;
	int startx = (COLS - NB_COLS * WIDTH) / 2;

	clear();

	display_candidate(startx, starty, state->candidate_pos, state->turn);
	display_board(startx, starty);
	display_discs(startx, starty, state->board);
	display_instructions();
	
	if (state->winner != NO_PLAYER)
		mvprintw(LINES - 2, 1, "Victory of player %d", state->winner);

	refresh();
}

void sig_handler(int sig) {
  endwin();
	printf("Tchau!\n");
	exit(0);
}