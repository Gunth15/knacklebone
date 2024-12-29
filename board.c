#include "knackle.h"
#include <stdio.h>
#include <stdlib.h>
#define SQUARE_SIZE 120
#define DEBUG 0
#if DEBUG
#define printdslots(ROW, COL, X, Y)                                            \
  printf("DiceSlot %d,%d: x:%d y:%d\n", COL, ROW, X, Y)
#define printdmouse(X_MOUSE, Y_MOUSE, COL)                                     \
  printf("Mouse in square with pos x:%d, y:%d in the %d column\n", X_MOUSE,    \
         Y_MOUSE, COL)
#else
#define printdslots(ROW, COL, X, Y)
#define printdmouse(X_MOUSE, Y_MOUSE, COL)
#define printdslotclicked(COL, ROll)
#endif /* if DEBUG */

void InitBoard(DiceSlot *board[], int size, int player) {
  int dif = 1;
  if (player == 2) {
    dif = 4;
  }
  for (int col = 0; col < size; ++col) {
    board[col] = (DiceSlot *)malloc(sizeof(DiceSlot) * size * 2);

    if (board[col] == NULL) {
      fprintf(stderr, "Could not allocate memory for board\n");
      exit(EXIT_FAILURE);
    }

    for (int row = 0; row < size; ++row) {
      int x = (GetScreenWidth() - (SQUARE_SIZE + 2) * size) / 2 +
              (col * (SQUARE_SIZE + 2));
      int y = GetScreenHeight() / (2 * dif) + (row * (SQUARE_SIZE + 2));

      DiceSlot slot = {0, x, y, SQUARE_SIZE};

      printdslots(row, col, x, y);

      board[col][row] = slot;
    }
  }
}

void ResizeBoard(DiceSlot **board, int size, int player) {
  int dif = 1;
  if (player == 2) {
    dif = 4;
  }
  for (int col = 0; col < size; ++col) {
    for (int row = 0; row < size; ++row) {
      int x = (GetScreenWidth() - (SQUARE_SIZE + 2) * size) / 2 +
              (col * (SQUARE_SIZE + 2));
      int y = GetScreenHeight() / (2 * dif) + (row * (SQUARE_SIZE + 2));

      printdslots(row, col, x, y);

      DiceSlot slot = board[col][row];
      slot.x = x, slot.y = y;
    }
  }
}

static int column_number;

bool IsMouseOnBoard(DiceSlot *board[], int size) {
  column_number = 0;
  int slot_size = board[0][0].size;
  int min_y = board[0][0].y;
  int max_y = board[size - 1][size - 1].y + slot_size;
  int x_mouse_pos = GetMouseX();
  int y_mouse_pos = GetMouseY();

  for (; column_number < size; ++column_number) {
    int max_x = board[column_number][0].x + slot_size;
    int min_x = board[column_number][0].x;
    if (x_mouse_pos > min_x && x_mouse_pos < max_x && y_mouse_pos < max_y &&
        y_mouse_pos > min_y) {
      printdmouse(x_mouse_pos, y_mouse_pos, column_number);
      return 1;
    }
  };
  return 0;
}

int GetMouseColumn() { return column_number; }

bool PlaceRoll(DiceSlot *board[], int size, int col, int roll) {
  int die;
  for (die = 0; die < size && board[col][die].value != 0; ++die)
    ;
  if (die < size) {
    board[col][die].value = roll;

    return 1;
  } else
    return 0;
}

void CheckDups(DiceSlot *enemy_board[], int roll, int col, int size) {
  int match;
  for (int i = 0; i < size; ++i) {
    if (enemy_board[col][i].value == roll) {
      enemy_board[col][i].value = 0;
    }
  }
}
