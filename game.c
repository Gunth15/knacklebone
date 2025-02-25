#include "knackle.h"
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#define BOARD_SIZE 3

//  TODO: Make win/lose screen

int main(void) {
  int screenwidth = 0;
  int screenheight = 0;
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(screenwidth, screenheight, "Knacklebone");
  DiceImg img = InitDiceImg();

  bool game_over = 0; // signifies end of game
  bool winner = 0;    // winnner 0 = p1, 1 = p2
  bool mouse_state = 0;
  bool is_mouse_on_board;
  bool roll = 0;
  bool turn = 1; // 1=player turn, 0=cpu turn
  int dice = GetRandomValue(1, 6);

  ColScore score1[BOARD_SIZE];
  ColScore score2[BOARD_SIZE];
  DiceSlot *board1[BOARD_SIZE] = {0};
  DiceSlot *board2[BOARD_SIZE] = {0};
  InitBoard(board1, BOARD_SIZE, 1);
  InitBoard(board2, BOARD_SIZE, 2);
  InitScoreBoardBelow(score1, board1, BOARD_SIZE);
  InitScoreBoardAbove(score2, board2, BOARD_SIZE);

  while (!WindowShouldClose()) {
    //-----------------------Resize-Check-------------------------------------------
    if (screenwidth != GetScreenWidth() || screenheight != GetScreenHeight()) {
      ResizeBoard(board1, BOARD_SIZE, 1);
      ResizeBoard(board2, BOARD_SIZE, 2);
      ResizeScoreBelow(score1, board1, BOARD_SIZE);
      ResizeScoreAbove(score2, board2, BOARD_SIZE);
      screenwidth = GetScreenWidth();
      screenheight = GetScreenHeight();
    }
    if (!game_over) {
      //-------------------Player-Turn-----------------------------------------------
      if (turn) {
        //-------------------Click-Phase-----------------------------------------------
        is_mouse_on_board = IsMouseOnBoard(board1, BOARD_SIZE);
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
          mouse_state = is_mouse_on_board ? 1 : 0;
        }
        if (IsMouseButtonUp(MOUSE_BUTTON_LEFT) && mouse_state) {
          int col = GetMouseColumn();
          if (!PlaceRoll(board1, BOARD_SIZE, col, dice)) {
            printf("No\n");
          } else {
            UpdateScore(score1, board1, BOARD_SIZE);
            CheckDups(board2, dice, col, BOARD_SIZE);
            UpdateScore(score2, board2, BOARD_SIZE);
            turn = 0;
            roll = 0;
          }
          mouse_state = 0;
          if ((game_over = IsBoardFull(board1, BOARD_SIZE))) {
            winner = DecideWinner(score1, score2, BOARD_SIZE);
          };
        }
      }

      //-----------------------Roll-Phase---------------------------------------------
      if (!roll) {
        dice = GetRandomValue(1, 6);
        roll = 1;
      }

      //-------------------Computer-Turn-----------------------------------------------
      if (!turn) {
        // Convert board into something cpu can see
        int **cpu_board = malloc(sizeof(int *) * BOARD_SIZE);
        int **enemy_board = malloc(sizeof(int *) * BOARD_SIZE);
        for (int i = 0; i < BOARD_SIZE; i++) {
          enemy_board[i] = malloc(sizeof(int) * BOARD_SIZE);
          cpu_board[i] = malloc(sizeof(int) * BOARD_SIZE);
        }
        int *cpu_score = malloc(sizeof(int) * BOARD_SIZE);
        int *enemy_score = malloc(sizeof(int) * BOARD_SIZE);

        // Convert current board into arrays
        BoardToMatrix(board2, cpu_board, BOARD_SIZE);
        BoardToMatrix(board1, enemy_board, BOARD_SIZE);
        ScoreToMatrix(score1, enemy_score, BOARD_SIZE);
        ScoreToMatrix(score2, cpu_score, BOARD_SIZE);

        // Make state
        GameState state = {.cpu_board = cpu_board,
                           .enemy_board = enemy_board,
                           .enemy_score = enemy_score,
                           .cpu_score = cpu_score,
                           .size = BOARD_SIZE};

        int move = MakeNextMove(&state, dice, 1);
        PlaceRoll(board2, BOARD_SIZE, move, dice);
        UpdateScore(score2, board2, state.size);
        CheckDups(board1, dice, move, BOARD_SIZE);
        UpdateScore(score1, board1, state.size);
        if ((game_over = IsBoardFull(board2, BOARD_SIZE))) {
          winner = DecideWinner(score1, score2, BOARD_SIZE);
        };

        // free memory
        for (int i = 0; i < BOARD_SIZE; i++) {
          free(enemy_board[i]);
          free(cpu_board[i]);
        }
        free(enemy_board);
        free(cpu_board);
        free(cpu_score);
        free(enemy_score);

        turn = 1;
        roll = 0;
      }
    }
    //------------------------END-GAME-CLEANUP----------------------------------------------
    else {
      if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && !mouse_state) {
        mouse_state = 1;
      }
      if (IsMouseButtonUp(MOUSE_BUTTON_LEFT) && mouse_state) {
        InitBoard(board1, BOARD_SIZE, 1);
        InitBoard(board2, BOARD_SIZE, 2);
        InitScoreBoardBelow(score1, board1, BOARD_SIZE);
        InitScoreBoardAbove(score2, board2, BOARD_SIZE);
        game_over = 0;
        mouse_state = 0;
      }
    }

    //---------------------Draw-Phase-----------------------------------------------
    BeginDrawing();
    ClearBackground(GRAY);
    if (is_mouse_on_board) {
      AltColumnColor(board1, &img, GetMouseColumn(), BOARD_SIZE);
    } else {
      DrawBoard(board1, &img, BOARD_SIZE);
    }
    DrawDiceWindow(&img, dice);
    DrawBoard(board2, &img, BOARD_SIZE);
    DrawScore(score1, BOARD_SIZE);
    DrawScore(score2, BOARD_SIZE);
    // Draw Game over screen
    if (game_over) {
      DrawText("Game Over", GetScreenWidth() / 2 - 200,
               GetScreenHeight() / 2 - 100, 100, BLACK);
      if (winner) {
        DrawText("You Lose", GetScreenWidth() / 2 - 200, GetScreenHeight() / 2,
                 100, BLACK);
      } else {
        DrawText("You Win!", GetScreenWidth() / 2 - 200, GetScreenHeight() / 2,
                 100, BLACK);
      }
      DrawText("Click to Restart", GetScreenWidth() / 2 - 200,
               GetScreenHeight() / 2 + 100, 40, BLACK);
    }
    EndDrawing();
  }

  CloseWindow();

  return 0;
}
