#include "knackle.h"
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#define PRIMARY_COLOR BLUE
#define ALT_COLOR LIGHTGRAY
#define TEXT_COLOR BLACK

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

void DrawScore(ColScore *score, int size) {

  char *temp = malloc(3);
  for (int i = 0; i < size; ++i) {
    sprintf(temp, "%u\n", score[i].value);
    DrawText(temp, score[i].x, score[i].y, 30, TEXT_COLOR);
  }
  free(temp);
};

void DrawDiceWindow(int roll) {
  Rectangle container = {
      .x = 50,
      .y = 50,
      .height = 500,
      .width = 200,
  };
  char *temp = malloc(3);
  sprintf(temp, "%d", roll);
  DrawText(temp, container.width / 2 + container.x,
           container.height / 2 + container.y, 100, BLACK);
  free(temp);
  DrawRectangleRoundedLines(container, 90.0, 3, BLUE);
}
