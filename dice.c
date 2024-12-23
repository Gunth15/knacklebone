#include "knackle.h"
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>

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
