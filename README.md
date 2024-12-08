# KnackleBone

Small Raylib project that is inspired by Cult of The Lamb.

## Rules

- The game consists of two 3x3 boards, each belonging to their respective player.
- On a player's turn, they roll a single 6-sided die, and must place it in a column on their board. A filled column does not accept any more dice.
- Each player has a score, which is the sum of all the dice values on their board. The score awarded by each column is also displayed.
- If a player places multiple dice of the same value in the same column, the score awarded for each of those dice is multiplied by the number of dice of the same value in that column. e.g. if a column contains 4-1-4, then the score for that column is 4x2 + 1x1 + 4x2 = 17. Below is a multiplication table for reference and comparison:
- When a player places a die, all dice of the same value in the corresponding column of the opponent's board gets destroyed. Players can use this mechanic to destroy their opponent's high-scoring combos.
- The game ends when either player completely fills up their 3x3 board. The player with the higher score wins.

[Rules based on description on cult-of-the-lamb wiki](https://cult-of-the-lamb.fandom.com/wiki/Knucklebones)

## To Compile

### Dependencies

- Raylib, need to find a way to compile without cmake for windows and unix based systems

### Compile

Use command

```Bash
cc game.c board.c -lraylib -lGL -lm -lpthread  -lrt -lX11
```
