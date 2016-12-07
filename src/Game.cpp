#include <avr/io.h>
#include <Wire.h>
//include <time.h>
#include <stdlib.h>
#include <SPI.h>
#include <GraphicsLib.h>
#include <MI0283QT9.h>
#include <digitalWriteFast.h>
#include <stdbool.h>
#include <Arduino.h>
#include <util/delay.h>
#include <EEPROM.h>
#include <avr/interrupt.h>
#include <stdint.h>

#define maxBlocksInLength 4
#define maxBlocksInWidth 5
#define NELEMS(x)  (sizeof(x) / sizeof((x)[0]))

typedef struct {
  int x;
  int y;
  int explodeIn;
  bool placed;
  bool exploded;
} BOMB;

typedef struct {
  int x;
  int y;
  int lives;
  uint16_t color;
  BOMB *bomb;
} PLAYER;

typedef struct {
  uint8_t x;
  uint8_t y;
  bool buttonZ;
} NUNCHUCK_DATA;

NUNCHUCK_DATA nData;

MI0283QT9 lcd;
PLAYER player1;

String gameName = "Bomberman";
bool inGame = false;
bool inSettings = false;
bool rendering = true;

uint8_t teller = 0;
ISR(TIMER2_OVF_vect) {
  teller++;
  if ( teller >= 60 ) {
    teller = 0;
    if ((player1.bomb->placed || player1.bomb->exploded) && player1.bomb->explodeIn != 0) {
      player1.bomb->explodeIn--;
    }
  }
}

// void writeCalData(void)
// {
//   uint16_t i, addr=0;
//   uint8_t *ptr;
//
//   EEPROM.write(addr++, 0xAA);
//
//   ptr = (uint8_t*)&lcd.tp_matrix;
//   for(i=0; i < sizeof(CAL_MATRIX); i++)
//   {
//     EEPROM.write(addr++, *ptr++);
//   }
//
//   return;
// }
//
// Read the calibration data from EEPROM for the touch input

uint8_t readCalData(void)
{
  uint16_t i, addr=0;
  uint8_t *ptr;
  uint8_t c;

  c = EEPROM.read(addr++);
  if(c == 0xAA) // Start byte for calibration data
  {
    ptr = (uint8_t*)&lcd.tp_matrix;
    for(i=0; i < sizeof(CAL_MATRIX); i++) // Loop trough the calibration data in EEPROM and store it in the CAL_MATRIX of the GraphicsLib
    {
      *ptr++ = EEPROM.read(addr++);
    }
    return 0;
  }

  return 1;
}

void nunchuck_init() {
  Wire.begin();
  Wire.beginTransmission(0x52);       // device address
  Wire.write(0xF0);
  Wire.write(0x55);
  Wire.endTransmission();
  _delay_ms(1);
  Wire.beginTransmission(0x52);
  Wire.write(0xFB);
  Wire.write(0x00);
  Wire.endTransmission();
}


void nunchuck_get_data() {
  uint8_t cnt;
  uint8_t status[6];
  cnt = 0;
  Wire.requestFrom (0x52, 6); // request data from nunchuck
  while (Wire.available ()) {
      // receive byte as an integer
      status[cnt] =  Wire.read();
      cnt++;
  }
  if (cnt > 5) {
    nData.x = (status[0]);
    nData.y = (status[1]);
    nData.buttonZ = !(status[5] & B00000001);
  }

  // Send one byte of 00000000 to request next bytes
  Wire.beginTransmission(0x52);      // transmit to device 0x52
  Wire.write(0x00);           // sends one byte
  Wire.endTransmission();    // stop transmitting
}

void placePlayers() {
  // Init player 1
  player1.x = 0;
  player1.y = 0;
  player1.lives = 3;
  player1.color = RGB(36, 41, 117);

  player1.bomb = (BOMB *) malloc(sizeof(BOMB));
  player1.bomb->placed = false;

  //Draw player
  lcd.fillRect(player1.x + 7, player1.y + 8, 10, 10, RGB(64, 75, 230));
  lcd.fillRect(player1.x + 2, player1.y + 3, 20, 20, player1.color);
}

void initMenu() {
  // Remove every content on screen
  lcd.fillScreen(RGB(0,0,0));
  lcd.drawText(20, 20, gameName, RGB(255,255,255), RGB(0,0,0), 4);

  String startText = "Start game";
  lcd.drawText(40, 100, startText, RGB(255,255,255), RGB(30,130,76), 3);

  String settingsTxt = "Settings";
  lcd.drawText(40, 150, settingsTxt, RGB(255,255,255), RGB(34,167,240), 3);

}

void initSettings() {
  lcd.fillScreen(RGB(0,0,0));
  String title = "Settings";
  lcd.drawText(20, 20, title, RGB(255,255,255), RGB(0,0,0), 3);

  String backText = "Terug";
  lcd.drawText(230, 220, backText, RGB(255,255,255), RGB(0,0,0), 2);

  String brightnessTitle = "Brightness:";
  lcd.drawText(15, 80, brightnessTitle, RGB(255,255,255), RGB(0,0,0), 2);

  String turnUpBrightness = "Turn up";
  lcd.drawText(15, 120, turnUpBrightness, RGB(255,255,255), RGB(0,153,0), 2);

  String turnDownBrightness = "Turn down";
  lcd.drawText(15, 170, turnDownBrightness, RGB(255,255,255), RGB(153,0,0), 2);
}

int temp [30][2];
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
};

void spawnCrates(int crate){
  // Spawn crates on random locations
  randomSeed(analogRead(0));
  // Generate random numbers
  int r1 = random(66);
  bool isUnique = true;
  // Check if random numbers are unique
    for (int i = 0; i < 66; i++) {
      if (temp[i][0] == possiblePositions[r1][0] && temp[i][1] == possiblePositions[r1][1]) {
        spawnCrates(crate);
        isUnique = false;
      }
    }

    if (isUnique) {
      temp[crate][0] = possiblePositions[r1][0];
      temp[crate][1] = possiblePositions[r1][1];
    }

    // Draw crates
    lcd.fillRect(temp[crate][1] * 25 - 25, temp[crate][0] * 27 - 27, 25, 27, RGB(139,69,19));
    lcd.fillRect(temp[crate][1] * 25 - 22, temp[crate][0] * 27 - 24, 19, 21, RGB(160,82,45));
  }

void updateScore(){
  if(player1.lives == 3){
    lcd.fillCircle(260, 45, 4, RGB(204, 0, 0));
    lcd.fillCircle(275, 45, 4, RGB(204, 0, 0));
    lcd.fillCircle(290, 45, 4, RGB(204, 0, 0));
  } else if(player1.lives == 2){
    lcd.fillCircle(260, 45, 4, RGB(204, 0, 0));
    lcd.fillCircle(275, 45, 4, RGB(204, 0, 0));
  } else  if(player1.lives == 1){
    lcd.fillCircle(260, 45, 4, RGB(204, 0, 0));
  } else if(player1.lives == 0){
    lcd.fillCircle(260, 45, 4, RGB(47, 79, 79));
    lcd.fillCircle(275, 45, 4, RGB(47, 79, 79));
    lcd.fillCircle(290, 45, 4, RGB(47, 79, 79));
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

  // Draw text
  String startText = "Lifes:";
  lcd.drawText(256, 6, startText, RGB(47,79,79), RGB(0,0,0), 1);

  String player1 = "Player1";
  lcd.drawText(256, 27, player1, RGB(47,79,79), RGB(0,0,0), 1);
  // Spawn all the crates
  for (int i = 0; i < 30; i++) {
    spawnCrates(i);
  }
  placePlayers();
  updateScore();
}

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
  }

  return 7;
}

int getPlayerYGrid(int y) {
  if (y >= 0 && y <= 27) {
    return 1;
  } else if (y >= 27 && y <= 54) {
    return 2;
  } else if (y >= 54 && y <= 81) {
    return 3;
  } else if (y >= 81 && y <= 108) {
    return 4;
  }

  return 5;
}

void movePlayer(PLAYER *p, int UpDown, int LeftRight) {
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

  lcd.fillRect(u + 2, l + 3, 20, 20, RGB(11, 102, 41));
  lcd.fillRect(p->x + 2, p->y + 3, 20, 20, p->color);
  lcd.fillRect(p->x + 7, p->y + 8, 10, 10, RGB(0, 0, 204));

  if (UpDown == 1) {
    lcd.fillRect(p->x + 18, p->y + 5, 4, 4, RGB(64, 75, 230));
    lcd.fillRect(p->x + 18, p->y + 17, 4, 4, RGB(64, 75, 230));
  } else if (UpDown == 0) {
    lcd.fillRect(p->x + 2, p->y + 5, 4, 4, RGB(64, 75, 230));
    lcd.fillRect(p->x + 2, p->y + 17, 4, 4, RGB(64, 75, 230));
  }else if (LeftRight == 1) {
    lcd.fillRect(p->x + 4, p->y + 19, 4, 4, RGB(64, 75, 230));
    lcd.fillRect(p->x + 16, p->y + 19, 4, 4, RGB(64, 75, 230));
  } else if (LeftRight == 0) {
    lcd.fillRect(p->x + 4, p->y + 3, 4, 4, RGB(64, 75, 230));
    lcd.fillRect(p->x + 16, p->y + 3, 4, 4, RGB(64, 75, 230));
  }
}

bool checkHitWall(PLAYER *p, int side) {
  if (((p->x + 20) == 245) && (side == 1)) {
    return true;
  }
  int walls[6][2] = {
    {2,2},
    {2,4},
    {2,6},
    {4,2},
    {4,4},
    {4,6}
  };

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

  for (int i = 0; i < 6; i++) {
    int x = walls[i][1];
    int y = walls[i][0];
      if (((xGridB == x) && (yGridB == y)) || ((xGridE == x) && (yGridE == y))) {
      return true;
    } else if (((xGridB == x) && (yGridB == y)) && ((xGridE == x) && (yGridE == y))) {
      return true;
    }

  }

  return false;
}

void placeBomb(PLAYER *p) {
  if (!p->bomb->placed) {
    p->bomb->placed = true;
    p->bomb->x = p->x;
    p->bomb->y = p->y;
    p->bomb->explodeIn = 5;
    p->bomb->exploded = false;
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
    player1.bomb->explodeIn = 3;
    // Draw explosion animation
    lcd.fillRect(player1.bomb->x + 10, player1.bomb->y, 14, 14, RGB(15, 101, 15));
    lcd.fillCircle(player1.bomb->x + 13, player1.bomb->y + 14, 8, RGB(209, 199, 57));
    lcd.fillCircle(player1.bomb->x + 13, player1.bomb->y + 14, 6, RGB(215, 84, 37));
    lcd.fillCircle(player1.bomb->x + 13, player1.bomb->y + 14, 3, RGB(215, 50, 37));
  } else if (player1.bomb->exploded && player1.bomb->explodeIn == 0) {
    player1.bomb->exploded = false;
    lcd.fillRect(player1.bomb->x, player1.bomb->y, 22, 23, RGB(11, 102, 41));
  }
}

void updatePlayers() {
  int UpDown = 6, LeftRight = 6;

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

int brightness = 5;
void turnBrightnessUp(int brightness){
    lcd.led(brightness * 10);
}

void turnBrightnessDown(int brightness){
    lcd.led(brightness * 10);
}

int main(void) {
  init();
  TCCR2B |= (1 << CS02) | (1<<CS00);
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
          if(brightness < 10){
            brightness += 2;
            turnBrightnessUp(brightness);
            _delay_ms(100);
          }
        } else if(touchX > 15 && touchX < 139 && touchY > 170 && touchY < 180){
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
      updatePlayers();
      updateBombs();
      rendering = false;
      _delay_ms(50);
    }

  }
  return 0;
}
