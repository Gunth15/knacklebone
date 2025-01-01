#include "knackle.h"
#include <stdio.h>
#include <stdlib.h>
#define DEBUG 1
#if DEBUG
#define printdMatrix(MATRIX, SIZE)                                             \
  for (int row = 0; row < size; ++row) {                                       \
    for (int col = 0; col < size; ++col)                                       \
      printf("%d", MATRIX[col][row]);                                          \
    printf("\n");                                                              \
  }
#define printdTotalWeight(WEIGHT) printf("Total weight: %f\n", WEIGHT)
#define printdBest(ROLL, COL, WEIGHT)                                          \
  printf("The best move is to place roll %d in the column %d with the weight " \
         "%f\n",                                                               \
         ROLL, COL, WEIGHT)
#define printdMove(ROLL, COL, MATRIX, SIZE)                                    \
  printf("Current Roll is in column %d with a roll value of %d\n\n CPU:\n",    \
         COL, ROLL);                                                           \
  printdMatrix(MATRIX, SIZE)
#define printdChance(COL, WEIGHT, MATRIX, SIZE)                                \
  printf("Node %d has weight of %f\n\n CPU:\n", COL, WEIGHT);                  \
  printdMatrix(MATRIX, SIZE)
#define printdOpponent(COL, WEIGHT, MATRIX, SIZE)                              \
  printf("Node %d has weight of %f\n\n ENEMY:\n", COL, WEIGHT);                \
  printdMatrix(MATRIX, SIZE)
#else
#define printdMatrix(MATRIX, SIZE)
#define printdweight(WEIGHT)
#define printdBest(WEIGHT, COL)
#define printdMove(MATRIX, SIZE) printdMatrix(MATRIX, SIZE)
#define printdChance(COL, WEIGHT, MATRIX, SIZE)                                \
  #define printdOpponent(ROLL, WEIGHT, MATRIX, SIZE)
#endif

// Invalid Col positions
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
    weight += (cpu_score[i] - enemey_score[i]);
  }
  return weight;
}

float ChanceNodeCpu(int **cpu_board, int **enemy_board, int *cpu_score,
                    int *enemy_score, int col, int size, int depth) {
  int die;
  float weight, node_weight, new_node_weight;
  // get next open die position
  for (die = 0; die < size && cpu_board[col][die] != 0; ++die)
    ;
  if (die >= size) {
    return INVALID_CPU_COL;
  }

  int **tempE = malloc(sizeof(int *) * size),
      **tempC = malloc(sizeof(int *) * size),
      *tempSE = malloc(sizeof(int *) * size),
      *tempSC = malloc(sizeof(int *) * size);
  for (int col = 0; col < size; ++col) {
    tempC[col] = malloc(sizeof(int) * size);
    tempE[col] = malloc(sizeof(int) * size);
  }

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
        node_weight = (float)SumScore(cpu_score, enemy_score, size);
      }
      weight += node_weight;
      printdChance(col, node_weight, cpu_board, size);
    }
  }
  printdTotalWeight(weight / 7.0);
  free(tempE), free(tempC), free(tempSE), free(tempSC);
  return weight / 7.0;
}

float ChanceNodeOponent(int **cpu_board, int **enemy_board, int *cpu_score,
                        int *enemy_score, int col, int size, int depth) {
  int die;
  float weight, node_weight, new_node_weight = 0.0;
  // get next open die position
  for (die = 0; die < size && enemy_board[col][die] != 0; ++die)
    ;
  if (die >= size) {
    return INVALID_OPONENT_COl;
  }

  int **tempE = malloc(sizeof(int *) * size),
      **tempC = malloc(sizeof(int *) * size),
      *tempSE = malloc(sizeof(int *) * size),
      *tempSC = malloc(sizeof(int *) * size);
  for (int col = 0; col < size; ++col) {
    tempC[col] = malloc(sizeof(int) * size);
    tempE[col] = malloc(sizeof(int) * size);
  }

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
      node_weight = (float)SumScore(cpu_score, enemy_score, size);
    }

    weight += node_weight;
    printdChance(col, node_weight, cpu_board, size);
  }
  free(tempE), free(tempC), free(tempSE), free(tempSC);
  return weight / 7.0;
}

int MakeNextMove(int **cpu_board, int **enemy_board, int *cpu_score,
                 int *enemy_score, int size, int roll, int depth) {
  int best_move = 0, die = 0;
  float best_child = 0.0, child = 0.0;

  ;
  int **tempE = malloc(sizeof(int *) * size),
      **tempC = malloc(sizeof(int *) * size),
      *tempSE = malloc(sizeof(int *) * size),
      *tempSC = malloc(sizeof(int *) * size);
  for (int col = 0; col < size; ++col) {
    tempC[col] = malloc(sizeof(int) * size);
    tempE[col] = malloc(sizeof(int) * size);
  }
  CopyMatrix(enemy_board, tempE, size);
  CopyMatrix(cpu_board, tempC, size);
  CopyArray(enemy_score, tempSE, size);
  CopyArray(cpu_score, tempSC, size);
  for (int col = 0; col < size; ++col) {
    for (die = 0; die < size && cpu_board[col][die] != 0; ++die)
      ;
    if (die >= size)
      ;
    else {
      CopyMatrix(tempE, enemy_board, size);
      CopyMatrix(tempC, cpu_board, size);
      CopyArray(tempSE, enemy_score, size);
      printf("Bro I hat that sound\n");
      CopyArray(tempSC, cpu_score, size);

      cpu_board[col][die] = roll;
      printdMove(roll, col, cpu_board, size);

      CheckDupsM(cpu_board, enemy_board[col][die], col, size);
      UpdateScoreM(enemy_score, enemy_board, size);
      UpdateScoreM(cpu_score, cpu_board, size);

      if ((child = ChanceNodeOponent(cpu_board, enemy_board, cpu_score,
                                     enemy_score, col, size, depth - 1)) >
          best_child) {

        best_child = child;
        best_move = col;
      }
    }
  }
  printdBest(roll, best_move, best_child);
  free(tempE), free(tempC), free(tempSE), free(tempSC);
  return best_move;
}
