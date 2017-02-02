#include "header.h"

void placePlayers() {
  // Init player 1
  player1.x = 0;
  player1.y = 0;
  player1.lives = 3;
  player1.color = RGB(0, 0, 0);

  //Allocate memory for bomb
  player1.bomb = (BOMB *) malloc(sizeof(BOMB));
  player1.bomb->placed = false;

  //Draw player
  lcd.fillCircle(player1.x + 12, player1.y + 12, 10, player1.color);
  lcd.fillCircle(player1.x + 12, player1.y + 12, 5, RGB(64, 75, 230));
}

void initGame() {
  // Remove every content on screen
  lcd.fillScreen(RGB(11, 102, 41));
  // Create all the walls
  for (int w = 0; w < maxBlocksInWidth; w++) {
    for (int i = 0; i < maxBlocksInLength; i++) {
      lcd.fillRect(w * 50 + 25, i * 54 + 27, 25, 27, RGB(105, 105, 105));
      lcd.fillRect(w * 50 + 27, i * 54 + 29, 21, 23, RGB(128, 128, 128));
      lcd.fillRect(w * 50 + 25, i * 54 + 27, 2, 27, RGB(150, 150, 150));
      lcd.fillRect(w * 50 + 25, i * 54 + 27, 25, 2, RGB(150, 150, 150));
    }
  }
  // Draw score section
  lcd.fillRect(250, 0, 70, 240, RGB(47,79,79));
  lcd.fillRect(253, 3, 64, 234, RGB(0,0,0));
  lcd.fillRect(253, 17, 64, 3, RGB(47,79,79));
  lcd.fillRect(253, 154, 64, 3, RGB(47,79,79));
  lcd.fillRect(253, 170, 64, 3, RGB(47,79,79));

  // Draw text
  String startText = "Lifes:";
  lcd.drawText(256, 6, startText, RGB(47,79,79), RGB(0,0,0), 1);

  String player1 = "Player1";
  lcd.drawText(256, 27, player1, RGB(47,79,79), RGB(0,0,0), 1);

  String score = "Score:";
  lcd.drawText(256, 160, score, RGB(47,79,79), RGB(0,0,0), 1);

  //Clear temp array
  for (int j =0 ; j < 30; j++){
    temp[j][0] = 0;
    temp[j][1] = 0;
  }
  // Spawn all the crates
int i = 30;
  spawnCrates(i);
  placePlayers();
  updateScore();
}
