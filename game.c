#include "knackle.h"
#include <raylib.h>
#include <stdio.h>
#define BOARD_SIZE 3

// TODO: Print the number randomly generated by presssed phase

int main(void) {
  InitWindow(1000, 1000, "raylib [core] example - basic window");

  bool mouse_state;
  bool is_mouse_on_board;

  DiceSlot *boards[BOARD_SIZE] = {0};
  InitBoard(boards, BOARD_SIZE);

  while (!WindowShouldClose()) {
    is_mouse_on_board = IsMouseOnBoard(boards, BOARD_SIZE);
    //-------------------Pressed-Phase-----------------------------------------------
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
      if (is_mouse_on_board)
        mouse_state = 1;
      else
        mouse_state = 0;
    }
    if (IsMouseButtonUp(MOUSE_BUTTON_LEFT) && mouse_state) {
      if (!RollDice(boards, BOARD_SIZE, GetMouseColumn()))
        printf("Could not roll dice. This will be replaced with a animation or "
               "something in the future\n");
      mouse_state = 0;
    }
    //---------------------Draw-Phase-----------------------------------------------
    BeginDrawing();
    ClearBackground(GRAY);
    if (is_mouse_on_board) {
      AltColumnColor(boards, GetMouseColumn(), BOARD_SIZE);
    } else {
      DrawBoards(boards, BOARD_SIZE);
    }
    EndDrawing();
  }

  CloseWindow();

  return 0;
}
