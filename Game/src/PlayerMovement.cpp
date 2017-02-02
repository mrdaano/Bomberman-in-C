#include "header.h"
void movePlayer(PLAYER *p, int UpDown, int LeftRight) {
  // Set the new position of a player
  int u = p->x;
  if (UpDown == 1) {
    u = p->x - 5;
  } else if (UpDown == 0) {
    u = p->x + 5;
  }

  int l = p->y;

  if (LeftRight == 1) {
    l = p->y - 5;
  } else if (LeftRight == 0) {
    l = p->y + 5;
  }

  //Draw player
  lcd.fillRect(u, l + 1, 23, 23, RGB(11, 102, 41));
  lcd.fillCircle(p->x + 12, p->y + 12, 10, p->color);
  lcd.fillCircle(p->x + 12, p->y + 12, 5, RGB(64, 75, 230));

  //Draw player animation according to movement
  if (UpDown == 1) {
    lcd.fillCircle(p->x + 19, p->y + 7, 2, RGB(64, 75, 230));
    lcd.fillCircle(p->x + 19, p->y + 18, 2, RGB(64, 75, 230));
  } else if (UpDown == 0) {
    lcd.fillCircle(p->x + 4, p->y + 7, 2, RGB(64, 75, 230));
    lcd.fillCircle(p->x + 4, p->y + 18, 2, RGB(64, 75, 230));
  }else if (LeftRight == 1) {
    lcd.fillCircle(p->x + 6, p->y + 20, 2, RGB(64, 75, 230));
    lcd.fillCircle(p->x + 17, p->y + 20, 2, RGB(64, 75, 230));
  } else if (LeftRight == 0) {
    lcd.fillCircle(p->x + 6, p->y + 5, 2, RGB(64, 75, 230));
    lcd.fillCircle(p->x + 17, p->y + 5, 2, RGB(64, 75, 230));
  }
}

void updatePlayers() {
  int UpDown = 6, LeftRight = 6;

  // Check the position of nuchuck
  bool moved = false;
  if (nData.x > 135 && player1.x+5 != 320) {
    player1.x = player1.x+5;
    if (checkHitWall(&player1, 1)) {
      player1.x = player1.x-5;
      return;
    }
    moved = true;
    UpDown = 1;
  } else if(nData.x < 125 && !(player1.x-5 < 0)) {
    player1.x = player1.x-5;
    if (checkHitWall(&player1, 2)) {
      player1.x = player1.x+5;
      return;
    }
    moved = true;
    UpDown = 0;
  } else if (nData.y < 121 && player1.y+29 < 240) {
    player1.y = player1.y+5;
    if (checkHitWall(&player1, 3)) {
      player1.y = player1.y-5;
      return;
    }
    moved = true;
    LeftRight = 1;
  } else if (nData.y > 131 && player1.y-2 > 0) {
    player1.y = player1.y-5;
    if (checkHitWall(&player1, 4)) {
      player1.y = player1.y+5;
      return;
    }
    moved = true;
    LeftRight = 0;
  }

  if (moved) {
    movePlayer(&player1, UpDown, LeftRight);
  }
  if (nData.buttonZ && !rendering) {
    placeBomb(&player1);
  }
}
