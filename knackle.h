#include "board.c"
#include <raylib.h>

// Draws both boards for each player grid must have same number of row and
// columns update diceslot position on every call.
void DrawBoards(DiceSlot *board[], int size);
