// Small functions set of functions that control how the dice is rendered
#include "knackle.h"
#include <raylib.h>

DiceImg InitDiceImg() {
  DiceImg img = {
      .position = {0.0f, 0.0f},
      .texture = LoadTexture("./dice/six sided die.png"),
  };
  return img;
};
void DrawDiceImg(DiceImg *img, int dice_num, Rectangle destination) {
  if (!dice_num) {
    return;
  }
  Vector2 origin = {0.0f, 0.0f};
  Rectangle framerec = {0.0f, 0.0f, (float)img->texture.width / 6,
                        (float)img->texture.height / 16};
  framerec.x = (float)img->texture.width / 6 * (dice_num - 1);
  DrawTexturePro(img->texture, framerec, destination, origin, 0.0f, WHITE);
};
