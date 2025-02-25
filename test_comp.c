#include "knackle.h"
#include <stdio.h>
int main() {
  int c1[3] = {5, 0, 0};
  int c2[3] = {0, 0, 0};
  int c3[3] = {2, 2, 0};
  int *cpu_board[3] = {c1, c2, c3};

  int e1[3] = {0, 0, 0};
  int e2[3] = {0, 0, 0};
  int e3[3] = {2, 2, 0};
  int *enemy_board[3] = {e1, e2, e3};

  int cpu_score[3] = {0, 0, 0};
  int enemy_score[3] = {0, 0, 0};

  GameState game = {cpu_board, enemy_board, cpu_score, enemy_score, 3};
  printf("Thinking ...\n");
  int best = MakeNextMove(&game, 5, 2);
  printf("Next move should be %d\n", best);
  return 1;
}
