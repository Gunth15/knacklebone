#include <raylib.h>

// Diseslot is reresented by one spot on the grid
typedef struct {
  unsigned int value;
  int x;
  int y;
  int size;
} DiceSlot;

// Initializes memory for one board with default vaues
void InitBoard(DiceSlot *board[], int size, int player);

// Draws a board for a player. Grid must have same number of rows and
// columns update diceslot position on every call.
void DrawBoards(DiceSlot *board[], int size);

// Alternates color of one column
void AltColumnColor(DiceSlot *board[], int target_col, int size);

// Checks if the mouse is on the board or not
bool IsMouseOnBoard(DiceSlot *board[], int size);

// Gets the column number that the mouse was determined to be over when
// IsCursorOnBoard was called
int GetMouseColumn();

// Rolls a dice for the selected column starting with first row from the top of
// the column. Returns 0 if the column is full.
bool RollDice(DiceSlot *board[], int size, int col);

// Represents the columns, socre and location
typedef struct {
  unsigned int value;
  int x;
  int y;
} ColScore;

// Initializes Scrore board
void InitScoreBoard(ColScore *scoreboard, DiceSlot **board, int size);

//  Updates the score of the score board. Based on the current status of the
//  dice board. For more info on the scoring system, look at the README.
void UpdateScore(ColScore *score, DiceSlot **board, int size);

// Draw the scorebord
void DrawScore(ColScore *score, int size);
