#include "header.h"

NUNCHUCK_DATA nData;

MI0283QT9 lcd;
PLAYER player1;

String gameName = "Bomberman";
bool inGame = false;
bool inSettings = false;
bool rendering = true;

String text;
String highscore = "0";
uint16_t sec = 0;
uint8_t teller = 0;

ISR(TIMER2_OVF_vect) {
  teller++;
  if ( teller >= 30 ) {
    teller = 0;
    if (inGame) {
      sec++;
      text = sec; // Set the seconds of game is playing
      if ((player1.bomb->placed || player1.bomb->exploded) && player1.bomb->explodeIn != 0) {
        player1.bomb->explodeIn--; // Couting down the explosion of the bomb
      }
    }
  }
}

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

int temp [30][2]; // Array to store the choosen crates for a single game
int possiblePositions[66][2] = {
  {1,3}, {1,4}, {1,5}, {1,6}, {1,7}, {1,8}, {1,9}, {1,10},
  {2,3}, {2,5}, {2,7}, {2,9},
  {3,1}, {3,2}, {3,3}, {3,4}, {3,5}, {3,6}, {3,7}, {3,8}, {3,9}, {3,10},
  {4,1}, {4,3}, {4,5}, {4,7}, {4,9},
  {5,1}, {5,2}, {5,3}, {5,4}, {5,5}, {5,6}, {5,7}, {5,8}, {5,9}, {5,10},
  {6,1}, {6,3}, {6,5}, {6,7}, {6,9},
  {7,1}, {7,2}, {7,3}, {7,4}, {7,5}, {7,6}, {7,7}, {7,8}, {7,9}, {7,10},
  {8,1}, {8,3}, {8,5}, {8,7}, {8,9},
  {9,1}, {9,2}, {9,3}, {9,4}, {9,5}, {9,6}, {9,7}, {9,8}, {9,9}
}; // All possible positions for a crate

void addCrate(int crate, int r1){
  // Add a crate to the temp array
    temp[crate][0] = possiblePositions[r1][0];
    temp[crate][1] = possiblePositions[r1][1];

  // Draw crate on screen
  lcd.fillRect(temp[crate][1] * 25 - 25, temp[crate][0] * 27 - 27, 25, 27, RGB(139,69,19));
  lcd.fillRect(temp[crate][1] * 25 - 22, temp[crate][0] * 27 - 24, 19, 21, RGB(160,82,45));
  lcd.fillTriangle(temp[crate][1] * 25 - 19, temp[crate][0] * 27 - 23,
                   temp[crate][1] * 25 - 6, temp[crate][0] * 27 - 8,
                   temp[crate][1] * 25 - 6, temp[crate][0] * 27 - 22, RGB(139,69,19));
  lcd.fillTriangle(temp[crate][1] * 25 - 20, temp[crate][0] * 27 - 20,
                   temp[crate][1] * 25 - 20, temp[crate][0] * 27 - 6,
                   temp[crate][1] * 25 - 7, temp[crate][0] * 27 - 5, RGB(139,69,19));
}

void spawnCrates(int crate){
  // Spawn crates on random locations
  randomSeed(analogRead(0));
  // Generate random numbers
  int r1 = random(66);
  bool notFound = true;
  int times = 0;
  // Check if random numbers are unique
  while(notFound && times < 30){
    if(r1 > 66){
      r1 = 0;
    }
    if (temp[times][0] == possiblePositions[r1][0] && temp[times][1] == possiblePositions[r1][1]) {
      r1++;
      times++;
    }else{
      addCrate(crate, r1);
      notFound = false;
    }
  }
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
  for (int i = 0; i < 30; i++) {
    spawnCrates(i);
  }
  placePlayers();
  updateScore();
}

// Get the grid x position of a player or bomb
int getPlayerXGrid(int x) {
  if (x >= 0 && x <= 25) {
    return 1;
  } else if (x >= 25 && x <= 50) {
    return 2;
  } else if (x >= 50 && x <= 75) {
    return 3;
  } else if (x >= 75 && x <= 100) {
    return 4;
  } else if (x >= 100 && x <= 125) {
    return 5;
  } else if (x >= 125 && x <= 150) {
    return 6;
  } else if (x >= 150 && x <= 175) {
    return 7;
  } else if (x >= 175 && x <= 200) {
    return 8;
  } else if (x >= 200 && x <= 225) {
    return 9;
  }

  return 10;
}

// Get the grid y position of a player or bomb
int getPlayerYGrid(int y) {
  if (y >= 0 && y <= 27) {
    return 1;
  } else if (y >= 27 && y <= 54) {
    return 2;
  } else if (y >= 54 && y <= 81) {
    return 3;
  } else if (y >= 81 && y <= 108) {
    return 4;
  } else if (y >= 108 && y <= 135) {
    return 5;
  } else if (y >= 135 && y <= 162) {
    return 6;
  } else if (y >= 162 && y <= 189) {
    return 7;
  }

  return 8;
}

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

bool checkHitWall(PLAYER *p, int side) {
  if (((p->x + 20) == 245) && (side == 1)) {
    return true;
  }
  //Wall coordinates
  int walls[20][2] = {
    {2,2}, {2,4}, {2,6}, {2,8}, {2,10},
    {4,2}, {4,4}, {4,6}, {4,8}, {4,10},
    {6,2}, {6,4}, {6,6}, {6,8}, {6,10},
    {8,2}, {8,4}, {8,6}, {8,8}, {8,10}
  };

  //Get player position
  int xGridB = 0, yGridB = 0;
  int xGridE = 0, yGridE = 0;
  if (side == 1) {
    xGridB = getPlayerXGrid(p->x + 25);
    yGridB = getPlayerYGrid(p->y);
    xGridE = getPlayerXGrid(p->x + 25);
    yGridE = getPlayerYGrid(p->y + 26);
  } else if (side == 2) {
    xGridB = getPlayerXGrid(p->x);
    yGridB = getPlayerYGrid(p->y);
    xGridE = getPlayerXGrid(p->x);
    yGridE = getPlayerYGrid(p->y + 25);
  } else if (side == 3) {
    xGridB = getPlayerXGrid(p->x + 1);
    yGridB = getPlayerYGrid(p->y + 25);
    xGridE = getPlayerXGrid(p->x + 24);
    yGridE = getPlayerYGrid(p->y + 26);
  } else if (side == 4) {
    xGridB = getPlayerXGrid(p->x + 1);
    yGridB = getPlayerYGrid(p->y);
    xGridE = getPlayerXGrid(p->x + 24);
    yGridE = getPlayerYGrid(p->y);
  }

  //Check if player is hitting a wall
  for (int i = 0; i < 20; i++) {
    int x = walls[i][1];
    int y = walls[i][0];
      if (((xGridB == x) && (yGridB == y)) || ((xGridE == x) && (yGridE == y))) {
      return true;
    } else if (((xGridB == x) && (yGridB == y)) && ((xGridE == x) && (yGridE == y))) {
      return true;
    }

  }
  //Check if player is hitting a crate
  for (int i = 0; i < 30; i++) {
    int x = temp[i][1];
    int y = temp[i][0];
      if (((xGridB == x) && (yGridB == y)) || ((xGridE == x) && (yGridE == y))) {
      return true;
    } else if (((xGridB == x) && (yGridB == y)) && ((xGridE == x) && (yGridE == y))) {
      return true;
    }

  }

  return false;
}

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

int main(void) {
  //Initialisation
  init();
  TCCR2B |= (1 << CS22)|(1 << CS21)|(1 << CS20);
  TIMSK2 |= (1<<TOIE0);
  TCNT2 = 0;
  sei();
  lcd.begin(4);
  nunchuck_init();
  Serial.begin(9600);

  // calibration for touchpanel
  readCalData();
  // //set touchpanel calibration data
  // lcd.touchRead();
  // if(lcd.touchZ() || readCalData()) //calibration data in EEPROM?
  // {
  //   lcd.touchStartCal(); //calibrate touchpanel
  //   writeCalData(); //write data to EEPROM
  // }

  initMenu();
  for (;;) {
    // If player is not in game and user touched the screen
    if (!inGame && lcd.touchRead() == 1) {
      int_least16_t touchX = lcd.touchX();
      int_least16_t touchY = lcd.touchY();
      // Check if user is pressing on the screen
      if (!inSettings) {
        if (touchX > 40 && touchX < 280 && touchY > 100 && touchY < 120) {
          initGame();
          inGame = true;
        } if (touchX > 40 && touchX < 250 && touchY > 150 && touchY < 170) {
          initSettings();
          inSettings = true;
        }
      } else {
        if((touchX > 15 && touchX < 110 && touchY > 120 && touchY < 130)){
          // Change the brightness
          if(brightness < 10){
            brightness += 2;
            turnBrightnessUp(brightness);
            _delay_ms(100);
          }
        } else if(touchX > 15 && touchX < 139 && touchY > 170 && touchY < 180){
          // Change the brightness
          if(brightness > 1){
            brightness -= 2;
            turnBrightnessDown(brightness);
            _delay_ms(100);
          }
        } else if (touchX > 230 && touchX < 300 && touchY > 220 && touchY < 240) {
          initMenu();
          inSettings = false;
        }
      }
    }

    if (inGame) {
      nunchuck_get_data();
      lcd.drawText(256, 180, text, RGB(47,108,49), RGB(0,0,0), 1);
      updatePlayers();
      updateBombs();
      rendering = false;
      _delay_ms(50);
    }

  }
  return 0;
}
