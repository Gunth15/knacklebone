#include "knackle.h"
#include <stdio.h>
#define DEBUG 1
#if DEBUG
#define debug_pos(COL, X, Y)
#else
#define debug_pos(COL, X, Y)                                                   \
  printf("The co-ord of the the %d column of the score board is x:%d y:%d\n",  \
         COL, X, Y)
#endif /* ifndef DEBUG */

void InitScoreBoardBelow(ColScore *scoreboard, DiceSlot **board, int size) {
  int last = size - 1;
  int x = board[0][last].x + board[0][last].size / 2;
  int y = board[0][last].y + board[0][last].size * (3.0 / 2.0);
  for (int i = 0; i < size; ++i) {
    scoreboard[i].x = x + (board[last][last].size * i);
    scoreboard[i].y = y;
    scoreboard[i].value = 0;
    debug_pos(i, scoreboard[i].x, scoreboard[i].y);
  }
}

void InitScoreBoardAbove(ColScore *scoreboard, DiceSlot **board, int size) {
  int x = board[0][0].x + board[0][0].size / 2;
  int y = board[0][0].y - board[0][0].size / (3.0 / 2.0);
  for (int i = 0; i < size; ++i) {
    scoreboard[i].x = x + (board[0][0].size * i);
    scoreboard[i].y = y;
    scoreboard[i].value = 0;
    debug_pos(i, scoreboard[i].x, scoreboard[i].y);
  }
}

void ResizeScoreBelow(ColScore *scoreboard, DiceSlot **board, int size) {
  int last = size - 1;
  int x = board[0][last].x + board[0][last].size / 2;
  int y = board[0][last].y + board[0][last].size * (3.0 / 2.0);
  for (int i = 0; i < size; ++i) {
    scoreboard[i].x = x + (board[last][last].size * i);
    scoreboard[i].y = y;
    debug_pos(i, scoreboard[i].x, scoreboard[i].y);
  }
}

void ResizeScoreAbove(ColScore *scoreboard, DiceSlot **board, int size) {
  int x = board[0][0].x + board[0][0].size / 2;
  int y = board[0][0].y - board[0][0].size / (3.0 / 2.0);
  for (int i = 0; i < size; ++i) {
    scoreboard[i].x = x + (board[0][0].size * i);
    scoreboard[i].y = y;
    debug_pos(i, scoreboard[i].x, scoreboard[i].y);
  }
}

void UpdateScore(ColScore *score, DiceSlot **board, int size) {
  for (int col = 0; col < size; ++col) {
    int map[7] = {0, 0, 0, 0, 0, 0, 0};
    int total = 0;
    for (int row = 0; row < size; ++row) {
      int key = board[col][row].value;
      ++map[key];
    }
    for (int i = 0; i < 7; ++i) {
      total += (map[i] * i) * map[i];
    }
    score[col].value = total;
  }
}

// Removes dups from specified board
void CheckDups(DiceSlot *enemy_board[], unsigned int roll, int col, int size) {
  for (int i = 0; i < size; ++i) {
    if (enemy_board[col][i].value == roll) {
      enemy_board[col][i].value = 0;
    }
  }
}

// 0 = p1 wins , 1 = p2 wins
bool DecideWinner(ColScore *p1_score, ColScore *p2_score, int size) {
  int p1 = 0;
  int p2 = 0;
  for (int i = 0; i < size; i++) {
    p1 += p1_score[i].value;
    p2 += p2_score[i].value;
  }
  printf("Player one has a score of %d and player 2 has a score of %d", p1, p2);
  return p1 <= p2;
}
