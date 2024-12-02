#include "knackle.h"
#include <raylib.h>
#define BOARD_SIZE 3

int main(void) {
  InitWindow(800, 450, "raylib [core] example - basic window");

  DiceSlot *boards[BOARD_SIZE] = {0};
  InitBoard(boards, BOARD_SIZE);

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(GRAY);
    if (IsMouseOnBoard(boards, BOARD_SIZE)) {
      AltColumnColor(boards, GetMouseColumn(), BOARD_SIZE);
    } else {
      DrawBoards(boards, BOARD_SIZE);
    }
    EndDrawing();
  }

  CloseWindow();

  return 0;
}
