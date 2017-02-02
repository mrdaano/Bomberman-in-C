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
