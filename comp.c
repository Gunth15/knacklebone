#include "knackle.h"
#include <raylib.h>

typedef enum {
  INVALID_OPONENT_COl = 200,
  INVALID_CPU_COL = -200,
} INVALID_COL;
float ChanceNodeOponent(int **cpu_board, int **enemy_board, int *cpu_score,
                        int *enemy_score, int col, int size, int depth);
// copies board's values to matrix, so they can be used in other operation wiht
// minimal information transfer
void BoardToMatrix(DiceSlot **board1, int **matrix, int size) {
  for (int col = 0; col < size; ++col)
    for (int row = 0; row < size; ++row)
      matrix[col][row] = board1[col][row].value;
}

// Turns score into single array wihtout positional data for reuse
void ScoreToMatrix(ColScore *score1, int *matrix, int size) {
  for (int col = 0; col < size; ++col)
    matrix[col] = score1[col].value;
}

// special version of UpdateScore that takes a integer array instead
void UpdateScoreM(int *score, int **board, int size) {
  for (int col = 0; col < size; ++col) {
    int map[7] = {0, 0, 0, 0, 0, 0, 0};
    int total = 0;
    for (int row = 0; row < size; ++row) {
      int key = board[col][row];
      ++map[key];
    }
    for (int i = 0; i < 7; ++i) {
      total += (map[i] * i) * map[i];
    }
    score[col] = total;
  }
}

// special version of CheckDups that uses a even matrix instead
void CheckDupsM(int *enemy_board[], int roll, int col, int size) {
  int match;
  for (int i = 0; i < size; ++i) {
    if (enemy_board[col][i] == roll) {
      enemy_board[col][i] = 0;
    }
  }
}

void CopyMatrix(int *matrix[], int *matrix_copy[], int size) {
  for (int col = 0; col < size; ++col) {
    for (int row = 0; row < size; ++row) {
      matrix_copy[col][row] = matrix[col][row];
    }
  }
};

void CopyArray(int *matrix, int *matrix_copy, int size) {
  for (int col = 0; col < size; ++col) {
    matrix_copy[col] = matrix[col];
  }
};

// calculates a node's weight using the scores of cpu and player
int SumScore(int *cpu_score, int *enemey_score, int size) {
  int weight;
  for (int i = 0; i < size; ++size) {
    weight += cpu_score[i] - enemey_score[i];
  }
  return weight;
}

float ChanceNodeCpu(int **cpu_board, int **enemy_board, int *cpu_score,
                    int *enemy_score, int col, int size, int depth) {
  int die;
  float weight, node_weight, new_node_weight;
  // get next open die position
  for (die = 0; die < size && cpu_board[die] != 0; ++die)
    ;
  if (die >= size) {
    return INVALID_CPU_COL;
  }

  int **tempE, **tempC, *tempSE, *tempSC;
  CopyMatrix(enemy_board, tempE, size);
  CopyMatrix(cpu_board, tempC, size);
  CopyArray(enemy_score, tempSE, size);
  CopyArray(cpu_score, tempSC, size);

  for (int i = 0; i < 7; ++i) {
    CopyMatrix(tempE, enemy_board, size);
    CopyMatrix(tempC, cpu_board, size);
    CopyArray(tempSE, enemy_score, size);
    CopyArray(tempSC, cpu_score, size);

    cpu_board[col][die] = i;
    CheckDupsM(enemy_board, cpu_board[col][die], col, size);
    UpdateScoreM(enemy_score, enemy_board, size);
    UpdateScoreM(cpu_score, cpu_board, size);
    if (depth > 0) {
      weight += (float)SumScore(cpu_score, enemy_score, size);
    } else {
      for (int ncol = 0; ncol < size; ++ncol)
        if ((new_node_weight = ChanceNodeOponent(
                 cpu_board, enemy_board, cpu_score, enemy_score, ncol, size,
                 depth - 1)) > node_weight)
          node_weight = new_node_weight;

      if (node_weight == INVALID_OPONENT_COl) {
        weight += (float)SumScore(cpu_score, enemy_score, size);
      } else {
        weight += node_weight;
      }
    }
  }
  return (float)weight / 7.0;
}

float ChanceNodeOponent(int **cpu_board, int **enemy_board, int *cpu_score,
                        int *enemy_score, int col, int size, int depth) {
  int die;
  float weight, node_weight, new_node_weight = 0.0;
  // get next open die position
  for (die = 0; die < size && enemy_board[die] != 0; ++die)
    ;
  if (die >= size) {
    return INVALID_OPONENT_COl;
  }

  int **tempE, **tempC, *tempSE, *tempSC;
  CopyMatrix(enemy_board, tempE, size);
  CopyMatrix(cpu_board, tempC, size);
  CopyArray(enemy_score, tempSE, size);
  CopyArray(cpu_score, tempSC, size);

  for (int i = 0; i < 7; ++i) {
    CopyMatrix(tempE, enemy_board, size);
    CopyMatrix(tempC, cpu_board, size);
    CopyArray(tempSE, enemy_score, size);
    CopyArray(tempSC, cpu_score, size);

    enemy_board[col][die] = i;
    CheckDupsM(cpu_board, enemy_board[col][die], col, size);
    UpdateScoreM(enemy_score, enemy_board, size);
    UpdateScoreM(cpu_score, cpu_board, size);
    for (int ncol = 0; ncol < size; ++ncol)
      if ((new_node_weight = ChanceNodeCpu(cpu_board, enemy_board, cpu_score,
                                           enemy_score, ncol, size, depth)) <
          node_weight)
        node_weight = new_node_weight;

    if (node_weight == INVALID_CPU_COL) {
      weight += (float)SumScore(cpu_score, enemy_score, size);
    } else {
      weight += node_weight;
    }
  }
  return weight / 7.0;
}
