#include "knackle.h"
#include <stdio.h>
#include <stdlib.h>
#define DEBUG 1
#if DEBUG
#define printdMatrix(MATRIX, SIZE)                                             \
  for (int row = 0; row < SIZE; ++row) {                                       \
    for (int col = 0; col < SIZE; ++col)                                       \
      printf("|\t%d\t|", MATRIX[col][row]);                                    \
    printf("\n");                                                              \
  }                                                                            \
  printf("\n")
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

// WARNING: The following does not che check if memeory is properly allocated
// after a call to malloc

// Invalid Col positions
typedef enum {
  INVALID_OPONENT_COl = 200,
  INVALID_CPU_COL = -200,
} INVALID_COL;

float ChanceNodeOponent(GameState *game, int col, int depth);

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

void CopyGameState(GameState *game, GameState *game_copy) {
  CopyMatrix(game->cpu_board, game_copy->cpu_board, game->size);
  CopyMatrix(game->enemy_board, game_copy->enemy_board, game->size);
  CopyArray(game->cpu_score, game_copy->cpu_score, game->size);
  CopyArray(game->enemy_score, game_copy->enemy_score, game->size);
};

// calculates a node's weight using the scores of cpu and player
int SumScore(int *cpu_score, int *enemey_score, int size) {
  int total = 0;
  for (int i = 0; i < size; ++i) {
    total += (cpu_score[i] - enemey_score[i]);
  }
  return total;
}

float ChanceNodeCpu(GameState *game, int col, int depth) {
  int die;
  float weight = 0.0, node_weight = INVALID_CPU_COL, new_node_weight = 0.0;
  // get next open die position
  for (die = 0; die < game->size && game->cpu_board[col][die] != 0; ++die)
    ;
  if (die >= game->size) {
    return INVALID_CPU_COL;
  }

  GameState temp = {.cpu_board = malloc(sizeof(int *) * game->size),
                    .enemy_board = malloc(sizeof(int *) * game->size),
                    .cpu_score = malloc(sizeof(int) * game->size),
                    .enemy_score = malloc(sizeof(int) * game->size),
                    .size = game->size};

  for (int col = 0; col < game->size; ++col) {
    temp.cpu_board[col] = malloc(sizeof(int) * game->size);
    temp.enemy_board[col] = malloc(sizeof(int) * game->size);
  }

  CopyGameState(game, &temp);

  for (int i = 1; i < 7; ++i) {

    game->cpu_board[col][die] = i;

    CheckDupsM(game->enemy_board, game->cpu_board[col][die], col, game->size);
    UpdateScoreM(game->enemy_score, game->enemy_board, game->size);
    UpdateScoreM(game->cpu_score, game->cpu_board, game->size);

    // If stop condition met, return weight of noede, else calculates opnents
    // next possible moves
    if (depth <= 0) {
      weight = (float)SumScore(game->cpu_score, game->enemy_score, game->size);
    } else {
      for (int ncol = 0; ncol < game->size; ++ncol)
        if ((new_node_weight = ChanceNodeOponent(game, ncol, depth - 1)) >
            node_weight)
          node_weight = new_node_weight;

      if (node_weight == INVALID_OPONENT_COl) {
        node_weight =
            (float)SumScore(game->cpu_score, game->enemy_score, game->size);
      }
      weight += node_weight;
      printdChance(col, node_weight, game->cpu_board, game->size);
      CopyGameState(&temp, game);
    }
  }
  printdTotalWeight(weight / 7.0);
  free(temp.enemy_board), free(temp.cpu_board), free(temp.cpu_score),
      free(temp.enemy_score);
  return weight / 7.0;
}

float ChanceNodeOponent(GameState *game, int col, int depth) {
  int die;
  float weight = 0.0, node_weight = INVALID_CPU_COL, new_node_weight = 0.0;
  // get next open die position
  for (die = 0; die < game->size && game->enemy_board[col][die] != 0; ++die)
    ;
  if (die >= game->size) {
    return INVALID_OPONENT_COl;
  }

  GameState temp = {.cpu_board = malloc(sizeof(int *) * game->size),
                    .enemy_board = malloc(sizeof(int *) * game->size),
                    .cpu_score = malloc(sizeof(int) * game->size),
                    .enemy_score = malloc(sizeof(int) * game->size),
                    .size = game->size};

  for (int col = 0; col < game->size; ++col) {
    temp.cpu_board[col] = malloc(sizeof(int) * game->size);
    temp.enemy_board[col] = malloc(sizeof(int) * game->size);
  }

  CopyGameState(game, &temp);

  for (int i = 1; i < 7; ++i) {

    game->enemy_board[col][die] = i;

    CheckDupsM(game->cpu_board, game->enemy_board[col][die], col, game->size);
    UpdateScoreM(game->enemy_score, game->enemy_board, game->size);
    UpdateScoreM(game->cpu_score, game->cpu_board, game->size);
    for (int ncol = 0; ncol < game->size; ++ncol) {
      if ((new_node_weight = ChanceNodeCpu(game, ncol, depth)) < node_weight)
        node_weight = new_node_weight;
    }

    if (node_weight == INVALID_CPU_COL) {
      node_weight =
          (float)SumScore(game->cpu_score, game->enemy_score, game->size);
    }

    printdOpponent(col, node_weight, game->enemy_board, game->size);
    weight += node_weight;
    CopyGameState(&temp, game);
  }

  free(temp.enemy_board), free(temp.cpu_board), free(temp.cpu_score),
      free(temp.enemy_score);
  return weight / 7.0;
}

int MakeNextMove(GameState *game, int roll, int depth) {
  int best_move = 0, die = 0;
  float best_child = INVALID_CPU_COL, child = 0.0;

  GameState temp = {.cpu_board = malloc(sizeof(int *) * game->size),
                    .enemy_board = malloc(sizeof(int *) * game->size),
                    .cpu_score = malloc(sizeof(int) * game->size),
                    .enemy_score = malloc(sizeof(int) * game->size),
                    .size = game->size};

  for (int col = 0; col < game->size; ++col) {
    temp.cpu_board[col] = malloc(sizeof(int) * game->size);
    temp.enemy_board[col] = malloc(sizeof(int) * game->size);
  }

  CopyGameState(game, &temp);

  for (int col = 0; col < game->size; ++col) {
    for (die = 0; (die < game->size) && (game->cpu_board[col][die] != 0); ++die)
      ;
    if (die >= game->size)
      ;
    else {

      game->cpu_board[col][die] = roll;
      printdMove(roll, col, game->cpu_board, game->size);

      CheckDupsM(game->enemy_board, game->cpu_board[col][die], col, game->size);
      UpdateScoreM(game->enemy_score, game->enemy_board, game->size);
      UpdateScoreM(game->cpu_score, game->cpu_board, game->size);

      if ((child = ChanceNodeOponent(game, col, depth - 1)) > best_child) {

        best_child = child;
        best_move = col;
      }
      CopyGameState(&temp, game);
    }
  }
  free(temp.enemy_board), free(temp.cpu_board), free(temp.cpu_score),
      free(temp.enemy_score);
  printdBest(roll, best_move + 1, best_child);
  return best_move;
}
