#include "header.h"

void placeBomb(PLAYER *p) {
  // Place a bomb on the position of the player
  if (!p->bomb->placed) {
    p->bomb->placed = true;
    p->bomb->x = p->x;
    p->bomb->y = p->y;
    p->bomb->explodeIn = 2;
    p->bomb->exploded = false;
  }
}

void updateBombDamage(BOMB *b) {
  int x = getPlayerXGrid(b->x + 7);
  int y = getPlayerYGrid(b->y + 7);

  int player1X = getPlayerXGrid(player1.x);
  int player1Y = getPlayerYGrid(player1.y);

  //Check if player is nearby bomb
  if ((player1X == x && player1Y == y) || (player1X == (x+1) && player1Y == y) || (player1X == (x-1) && player1Y == y) || (player1X == x && player1Y == (y+1)) || (player1X == x && player1Y == (y-1))) {
    player1.lives--;
    updateScore();
  }

  //Delete damagede crates
  for (int i = 0; i < 30; i++) {
    int crateY = temp[i][0];
    int crateX = temp[i][1];

    // Check if crate is on the right, left, top or bottom
    if ((crateX == (x+1) && crateY == y) || (crateX == (x-1) && crateY == y) || (crateX == x && crateY == (y + 1)) || (crateX == x && crateY == (y - 1))) {
      temp[i][0] = 0;
      temp[i][1] = 0;
      lcd.fillRect(crateX * 25 - 25, crateY * 27 - 27, 25, 27, RGB(11, 102, 41)); // Remove the crate from screen
      return;
    }
  }
}

void updateBombs() {
  if (player1.bomb->placed  && player1.bomb->explodeIn != 0) {
    //Draw bomb
    lcd.fillRect(player1.bomb->x + 10, player1.bomb->y + 6, 7, 3, RGB(45, 45, 51));
    lcd.fillCircle(player1.bomb->x + 13, player1.bomb->y + 14, 7, RGB(48, 48, 57));
    lcd.fillCircle(player1.bomb->x + 11, player1.bomb->y + 14, 3, RGB(66, 66, 75));
    lcd.fillRect(player1.bomb->x + 12, player1.bomb->y + 3, 2, 2, RGB(255, 255, 255));
    lcd.fillCircle(player1.bomb->x + 13, player1.bomb->y + 1, 1, RGB(215, 144, 37));
  } else if (player1.bomb->placed  && player1.bomb->explodeIn == 0) {
    player1.bomb->placed = false;
    player1.bomb->exploded = true;
    player1.bomb->explodeIn = 2;
    // Draw explosion animation
    lcd.fillRect(player1.bomb->x + 10, player1.bomb->y, 14, 14, RGB(15, 101, 15));
    lcd.fillCircle(player1.bomb->x + 13, player1.bomb->y + 14, 8, RGB(209, 199, 57));
    lcd.fillCircle(player1.bomb->x + 13, player1.bomb->y + 14, 6, RGB(215, 84, 37));
    lcd.fillCircle(player1.bomb->x + 13, player1.bomb->y + 14, 3, RGB(215, 50, 37));
    updateBombDamage(player1.bomb);
  } else if (player1.bomb->exploded && player1.bomb->explodeIn == 0) {
    // Remove explosion animation
    player1.bomb->exploded = false;
    if(player1.lives != 0){
      lcd.fillRect(player1.bomb->x, player1.bomb->y, 22, 23, RGB(11, 102, 41));
    } else
      lcd.fillRect(player1.bomb->x, player1.bomb->y, 22, 23, RGB(0, 0, 0));
    }
}
