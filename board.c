#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#define SQUARE_SIZE 150
#define DEBUG 0

typedef struct {
  unsigned int value;
  int x;
  int y;
  int size;
} DiceSlot;

void InitBoard(DiceSlot *board[], int size) {
  for (int row = 0; row < size; ++row) {
    board[row] = (DiceSlot *)malloc(sizeof(DiceSlot) * size * 2);

    if (board[row] == NULL) {
      fprintf(stderr, "Could not allocate memory for board\n");
      exit(EXIT_FAILURE);
    }

    for (int col = 0; col < size * 2; ++col) {
      int x = GetScreenWidth() / 2 + (row * (SQUARE_SIZE + 2));
      int y = GetScreenHeight() / 4 + (col * (SQUARE_SIZE + 2));

      DiceSlot slot = {0, x, y, SQUARE_SIZE};

#if DEBUG
      printf("DiceSlot %d,%d: x:%d y:%d\n", col, row, x, y);
#endif

      board[row][col] = slot;
    }
  }
}

void DrawBoards(DiceSlot *board[], int size) {
  for (int row = 0; row < size; ++row) {
    for (int col = 0; col < size * 2; ++col) {
#if DEBUG
      printf("Drawing at coords x:%d y:%d for row %d and col %d\n",
             board[row][col].x, board[row][col].y, row, col);
#endif
      DrawRectangle(board[row][col].x, board[row][col].y, board[row][col].size,
                    board[row][col].size, RED);
    }
  }
}

bool MouseOnSlot(DiceSlot *board[], int size)
