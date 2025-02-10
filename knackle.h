#include <raylib.h>

// Diseslot is reresents one square on the grid
typedef struct {
  unsigned int value;
  int x;
  int y;
  int size;
} DiceSlot;

// Represent generiec information for Dice img.
typedef struct {
  Texture2D texture;
  Vector2 position;
} DiceImg;

// Initializes memory for one board with default vaues
void InitBoard(DiceSlot *board[], int size, int player);

// Resize board if window size changes
void ResizeBoard(DiceSlot **board, int size, int player);

// Draws a board for a player. Grid must have same number of rows and
// columns update diceslot position on every call.
void DrawBoard(DiceSlot *board[], DiceImg *img, int size);

// Alternates color of one column
void AltColumnColor(DiceSlot *board[], DiceImg *img, int target_col, int size);

// Checks if the mouse is on the board or not
bool IsMouseOnBoard(DiceSlot *board[], int size);

// Gets the column number that the mouse was determined to be over when
// IsCursorOnBoard was called
int GetMouseColumn();

// Rolls a dice for the selected column starting with first row from the top of
// the column. Returns 0 if the column is full.
bool PlaceRoll(DiceSlot *board[], int size, int col, int roll);

// Check for values on the enemy board that equal the new roll and "delete"(set
// them to zero)
void CheckDups(DiceSlot *enemy_board[], unsigned int roll, int col, int size);

int RollDice();

// Represents the columns, socre and location
typedef struct {
  unsigned int value;
  int x;
  int y;
} ColScore;

// Initializes Scrore board
void InitScoreBoardBelow(ColScore *scoreboard, DiceSlot **board, int size);
void InitScoreBoardAbove(ColScore *scoreboard, DiceSlot **board, int size);

// Resize the scoreboard if window size changes
void ResizeScoreAbove(ColScore *scoreboard, DiceSlot **board, int size);
void ResizeScoreBelow(ColScore *scoreboard, DiceSlot **board, int size);

// Checks to see if board has open spaces
bool IsBoardFull(DiceSlot **board, int size);

//  Updates the score of the score board. Based on the current status of the
//  dice board. For more info on the scoring system, look at the README.
void UpdateScore(ColScore *score, DiceSlot **board, int size);

// Decides winner 0=p1, 1=p2
bool DecideWinner(ColScore *p1_score, ColScore *p2_score, int size);

// Draw the scorebord
void DrawScore(ColScore *score, int size);

void DrawDiceWindow(DiceImg *img, int roll);

// Turns dice board into matrix
void BoardToMatrix(DiceSlot **board1, int **matrix, int size);
// Turns score board into matrix
void ScoreToMatrix(ColScore *score1, int *matrix, int size);

// GameState represent the all variables needed that the CPU need to determine
// best move, except for the known roll
typedef struct {
  int **cpu_board;
  int **enemy_board;
  int *cpu_score;
  int *enemy_score;
  int size;
} GameState;

// Determines best move to make using a mix of expectimax and minimax algorithm
int MakeNextMove(GameState *game, int roll, int depth);

// Initializes png set for dice
DiceImg InitDiceImg();

// Draws the dice Image
void DrawDiceImg(DiceImg *img, int dice_num, Rectangle destination);
