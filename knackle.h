#include "board.c"
#include <raylib.h>

// Draws both boards for each player grid must have same number of row and
// columns update diceslot position on every call.
void DrawBoards(DiceSlot *board[], int size);
// Checks if the mouse is on the board or not
bool IsMouseOnBoard(DiceSlot *board[], int size);
// Alternates color of one column and sets the others to the priary color if the
// grid
void AltColumnColor(DiceSlot *board[], int target_col, int size);
// Gets the column number that the mouse was determined to be over when
// IsCursorOnBoard was called
int GetMouseColumn();
