#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#define SQUARE_SIZE 150
#define DEBUG 0
#define PRIMARY_COLOR RED
#define ALT_COLOR PINK

typedef struct {
  unsigned int value;
  int x;
  int y;
  int size;
} DiceSlot;

void InitBoard(DiceSlot *board[], int size) {
  for (int col = 0; col < size; ++col) {
    board[col] = (DiceSlot *)malloc(sizeof(DiceSlot) * size * 2);

    if (board[col] == NULL) {
      fprintf(stderr, "Could not allocate memory for board\n");
      exit(EXIT_FAILURE);
    }

    for (int row = 0; row < size; ++row) {
      int x = GetScreenWidth() / 2 + (col * (SQUARE_SIZE + 2));
      int y = GetScreenHeight() / 4 + (row * (SQUARE_SIZE + 2));

      DiceSlot slot = {0, x, y, SQUARE_SIZE};

#if DEBUG
      printf("DiceSlot %d,%d: x:%d y:%d\n", col, row, x, y);
#endif

      board[col][row] = slot;
    }
  }
}

void DrawBoards(DiceSlot *board[], int size) {
  for (int col = 0; col < size; ++col) {
    for (int row = 0; row < size; ++row) {
      DrawRectangle(board[col][row].x, board[col][row].y, board[col][row].size,
                    board[col][row].size, PRIMARY_COLOR);
    }
  }
}

static int column_number;

bool IsMouseOnBoard(DiceSlot *board[], int size) {
  column_number = 0;
  int y = board[0][0].y;
  int slot_size = board[0][0].size;
  int x_mouse_pos = GetMouseX();
  int y_mouse_pos = GetMouseY();

  for (; column_number < size; ++column_number) {
    int end_of_slot = board[column_number][0].x + slot_size;
    int start_of_slot = board[column_number][0].x;
    if (x_mouse_pos > start_of_slot && x_mouse_pos < end_of_slot &&
        y_mouse_pos < (slot_size * (size + 1)) && y_mouse_pos > y) {
#if DEBUG
      printf("Mouse in square with pods x:%d, y:%d\n", x_mouse_pos,
             y_mouse_pos);
#endif
      return 1;
    }
  };
  return 0;
}

int GetMouseColumn() { return column_number; }

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
    }
  }
}
