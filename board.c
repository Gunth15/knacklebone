#include "knackle.h"
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#define SQUARE_SIZE 120
#define DEBUG 0
#define PRIMARY_COLOR BLUE
#define ALT_COLOR LIGHTGRAY
#define TEXT_COLOR BLACK

#if DEBUG
#define printdslots(ROW, COL, X, Y)                                            \
  printf("DiceSlot %d,%d: x:%d y:%d\n", col, row, x, y)
#define printdmouse(X_MOUSE, Y_MOUSE)                                          \
  printf("Mouse in square with pos x:%d, y:%d\n", x_mouse_pos, y_mouse_pos)
#else
#define printdslots(ROW, COL, X, Y)
#define printdmouse(X_MOUSE, Y_MOUSE)
#endif /* if DEBUG */

void InitBoard(DiceSlot *board[], int size, int player) {
  for (int col = 0; col < size; ++col) {
    board[col] = (DiceSlot *)malloc(sizeof(DiceSlot) * size * 2);

    if (board[col] == NULL) {
      fprintf(stderr, "Could not allocate memory for board\n");
      exit(EXIT_FAILURE);
    }

    for (int row = 0; row < size; ++row) {
      int dif = 1;
      if (player == 2) {
        dif = 6;
      }
      int x = (GetScreenWidth() - (SQUARE_SIZE + 2) * size) / 2 +
              (col * (SQUARE_SIZE + 2));
      int y = GetScreenHeight() / (2 * dif) + (row * (SQUARE_SIZE + 2));

      DiceSlot slot = {0, x, y, SQUARE_SIZE};

      printdslots(row, col, x, y);

      board[col][row] = slot;
    }
  }
}

void DrawValue(DiceSlot *slot) {
  char *temp = malloc(3);
  if (temp == NULL)
    fprintf(stderr, "Could not allocate memory for drawing value");
  sprintf(temp, "%u\n", slot->value);

  int center_x = slot->x + slot->size / 2;
  int center_y = slot->y + slot->size / 2;
  DrawText(temp, center_x, center_y, 30, TEXT_COLOR);
  free(temp);
}

void DrawBoard(DiceSlot *board[], int size) {
  for (int col = 0; col < size; ++col) {
    for (int row = 0; row < size; ++row) {
      DrawRectangle(board[col][row].x, board[col][row].y, board[col][row].size,
                    board[col][row].size, PRIMARY_COLOR);
      DrawValue(&board[col][row]);
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
      printdmouse(x_mouse_pos, y_mouse_pos);
      return 1;
    }
  };
  return 0;
}

int GetMouseColumn() { return column_number; }

bool RollDice(DiceSlot *board[], int size, int col) {
  int die;
  for (die = 0; die < size && board[col][die].value != 0; ++die)
    ;
  if (die < size) {
    board[col][die].value = GetRandomValue(1, 6);
    return 1;
  } else
    return 0;
}

void AltColumnColor(DiceSlot *board[], int target_col, int size) {
  for (int col = 0; col < size; ++col) {
    for (int row = 0; row < size; ++row) {
      if (col == target_col) {
        DrawRectangle(board[col][row].x, board[col][row].y,
                      board[col][row].size, board[col][row].size, ALT_COLOR);
      } else {
        DrawRectangle(board[col][row].x, board[col][row].y,
                      board[col][row].size, board[col][row].size,
                      PRIMARY_COLOR);
      }
      DrawValue(&board[col][row]);
    }
  }
}
