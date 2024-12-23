#include "knackle.h"

int ComputeMove();

int PredictOponent(DiceSlot **board1, DiceSlot **board2, ColScore *score1,
                   ColScore *score2, int col, int size) {
  int worst;
  for (int roll = 0; roll < 7; ++roll) {
    PlaceRoll(board1, size, col, roll);
    UpdateScore(score1, board1, size);
  }
  return worst;
};

int ComputeMove(DiceSlot **board1, DiceSlot **board2, ColScore *score1,
                ColScore *score2, int size, int roll) {
  int best, cur_predict, best_col;
  for (int col = 0; col < size; ++col) {
    PlaceRoll(board2, size, col, roll);
    UpdateScore(score2, board2, size);
    if (best < (cur_predict = PredictOponent(board1, board2, score1, score2,
                                             col, size))) {
      best = cur_predict;
      best_col = col;
    }
  }
  return best_col;
};
