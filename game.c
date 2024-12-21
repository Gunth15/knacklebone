#include "knackle.h"
#include <raylib.h>
#include <stdio.h>
#define BOARD_SIZE 3

// TODO: Make score board for each player
// TODO: Add AI opponent

int main(void) {
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(0, 0, "Knacklebone");

  bool mouse_state;
  bool is_mouse_on_board;
  ColScore score[BOARD_SIZE];

  DiceSlot *board1[BOARD_SIZE] = {0};
  DiceSlot *board2[BOARD_SIZE] = {0};
  InitBoard(board1, BOARD_SIZE, 1);
  InitBoard(board2, BOARD_SIZE, 2);
  InitScoreBoard(score, board1, BOARD_SIZE);

  while (!WindowShouldClose()) {
    is_mouse_on_board = IsMouseOnBoard(board1, BOARD_SIZE);
    //-------------------Click-Phase-----------------------------------------------
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
      if (is_mouse_on_board)
        mouse_state = 1;
      else
        mouse_state = 0;
    }
    if (IsMouseButtonUp(MOUSE_BUTTON_LEFT) && mouse_state) {
      if (!RollDice(board1, BOARD_SIZE, GetMouseColumn()))
        printf("Could not roll dice. This will be replaced with a animation or "
               "something in the future\n");
      else
        UpdateScore(score, board1, BOARD_SIZE);
      mouse_state = 0;
    }

    //---------------------Draw-Phase-----------------------------------------------
    BeginDrawing();
    ClearBackground(GRAY);
    if (is_mouse_on_board) {
      AltColumnColor(board1, GetMouseColumn(), BOARD_SIZE);
      DrawBoard(board2, BOARD_SIZE);
    } else {
      DrawBoard(board1, BOARD_SIZE);
      DrawBoard(board2, BOARD_SIZE);
    }
    DrawScore(score, BOARD_SIZE);
    EndDrawing();
  }

  CloseWindow();

  return 0;
}
