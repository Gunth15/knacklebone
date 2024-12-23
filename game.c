#include "knackle.h"
#include <raylib.h>
#include <stdio.h>
#define BOARD_SIZE 3

// TODO: Make score board for each player
// TODO: Change dice mechanic, so player knows role before pressing column
// TODO: Add AI opponent

int main(void) {
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(0, 0, "Knacklebone");

  bool mouse_state = 0;
  bool is_mouse_on_board;
  bool roll = 0;
  int dice;
  ColScore score1[BOARD_SIZE];
  ColScore score2[BOARD_SIZE];

  DiceSlot *board1[BOARD_SIZE] = {0};
  DiceSlot *board2[BOARD_SIZE] = {0};
  InitBoard(board1, BOARD_SIZE, 1);
  InitBoard(board2, BOARD_SIZE, 2);
  InitScoreBoardBelow(score1, board1, BOARD_SIZE);
  InitScoreBoardAbove(score2, board2, BOARD_SIZE);

  while (!WindowShouldClose()) {
    if (!roll) {
      dice = GetRandomValue(1, 6);
      roll = 1;
    }
    is_mouse_on_board = IsMouseOnBoard(board1, BOARD_SIZE);
    //-------------------Click-Phase-----------------------------------------------
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
      mouse_state = is_mouse_on_board ? 1 : 0;
    }
    if (IsMouseButtonUp(MOUSE_BUTTON_LEFT) && mouse_state) {
      if (!PlaceRoll(board1, BOARD_SIZE, GetMouseColumn(), dice))
        printf("Could not roll dice. This will be replaced with a animation or "
               "something in the future\n");
      else
        UpdateScore(score1, board1, BOARD_SIZE);
      roll = 0;
      mouse_state = 0;
    }

    //---------------------Draw-Phase-----------------------------------------------
    BeginDrawing();
    ClearBackground(GRAY);
    if (is_mouse_on_board) {
      AltColumnColor(board1, GetMouseColumn(), BOARD_SIZE);
    } else {
      DrawBoard(board1, BOARD_SIZE);
    }
    DrawDiceWindow(dice);
    DrawBoard(board2, BOARD_SIZE);
    DrawScore(score1, BOARD_SIZE);
    DrawScore(score2, BOARD_SIZE);
    EndDrawing();
  }

  CloseWindow();

  return 0;
}
