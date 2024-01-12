#include "../include/ligue4.h"

int main(int argc, char **argv) {
  signal(SIGINT, sig_handler);

  initscr();
  if (!has_colors()) {
    endwin();
    printf("Seu terminal não suporta cores!\n");
    exit(1);
  }

  clear();
  noecho();
  curs_set(0);
  cbreak();
  keypad(stdscr, true);

  start_color();
  init_pair(PLAYER_1, COLOR_PLAYER_1, COLOR_BLACK);
  init_pair(PLAYER_2, COLOR_PLAYER_2, COLOR_BLACK);
  init_pair(BOARD, COLOR_BOARD, COLOR_BLACK);

  struct game_state state = {0};
  init_game(&state);

  // game exec
  int ch = 0;
  while (true) {
    update(&state, ch);
    redraw(&state);
    ch = getch();
  }
  return 0;
}
