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


#define maxBlocksInLength 2
#define maxBlocksInWidth 3

typedef struct {
  int x;
  int y;
  int explodeIn;
  bool placed;
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

ISR(TIMER2_OVF_vect) {
  if (player1.bomb->placed) {
    //lcd.fillRect(player1.bomb->x, player1.bomb->y, 10, 10, RGB(217, 30, 24));
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
  player1.x = 10;
  player1.y = 10;
  player1.lives = 3;
  player1.color = RGB(30,144,255);

  player1.bomb = (BOMB *) malloc(sizeof(BOMB));
  player1.bomb->placed = false;

  lcd.fillRect(player1.x, player1.y, 20, 20, player1.color);
}

void initMenu() {
  // Remove every content on screen
  lcd.fillScreen(RGB(0,0,0));
  lcd.drawText(20, 20, gameName, RGB(255,255,255), RGB(0,0,0), 4);

  String startText = "Start game";
  lcd.drawText(80, 100, startText, RGB(255,255,255), RGB(30,130,76), 2);

}
// double temp[16][2] = {};
// double coordinates[16][2] = {
//   // x, y
//   1, 3,
//   1, 4,
//   1, 5};
// void generateCrates() {
//   Serial.begin(9600);
//   randomSeed(analogRead(0));
//   int r1 = random(5);
//   int r2 = random(1, 4);
//
// }

void initGame() {
  // Remove every content on screen
  lcd.fillScreen(RGB(255,255,255));
  // Create all the walls
  for (int w = 0; w < maxBlocksInWidth; w++) {
    for (int i = 0; i < maxBlocksInLength; i++) {
      lcd.fillRect(w * 90 + 45, i * 96 + 48, 45, 48, RGB(0,0,0));
    }
  }
  // Create random crates
  double coordinates[4][6]= {45, 90, 135, 180, 225, 270,
                             0, 0, 0, 0, 0, 0,
                             96, 96, 96, 96, 96, 96,
                             192, 192, 192, 192, 192, 192};
  for (int d = 0; d < 10; d++) {
      Serial.begin(9600);
      randomSeed(analogRead(0));
      int r1 = random(5);
      int r2 = random(1, 4);
      lcd.fillRect(coordinates[0][r1], coordinates[r2][r1], 45, 48, RGB(139,69,19));
      lcd.fillRect(coordinates[0][r1] + 2, coordinates[r2][r1] + 2, 41, 44, RGB(160,82,45));
      _delay_ms(100);
  }

  placePlayers();
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
    l = p->y - 2;
  } else if (LeftRight == 0) {
    l = p->y + 2;
  }

  lcd.fillRect(u, l, 20, 20, RGB(255,255,255));
  lcd.fillRect(p->x, p->y, 20, 20, p->color);
}

void placeBomb(PLAYER *p) {
  p->bomb->placed = true;
  p->bomb->x = p->x;
  p->bomb->y = p->y;
  p->bomb->explodeIn = 3;
}

void updatePlayers() {
  bool moved = false;
  int UpDown = 6, LeftRight = 6;
  if (nData.x > 135 && player1.x+5 != 320) {
    player1.x = player1.x+5;
    moved = true;
    UpDown = 1;
  } else if(nData.x < 125 && player1.x-5 != 0) {
    player1.x = player1.x-5;
    moved = true;
    UpDown = 0;
  }

  if (nData.y > 131 && player1.y-2 > 0) {
    player1.y = player1.y-2;
    moved = true;
    LeftRight = 0;
  } else if (nData.y < 121 && player1.x-2 < 240) {
    player1.y = player1.y+2;
    moved = true;
    LeftRight = 1;
  }

  if (moved) {
    movePlayer(&player1, UpDown, LeftRight);
  }
  if (nData.buttonZ) {
    placeBomb(&player1);
  }
}


int main(void) {
  init();
  TCCR2B |= (1 << CS02) | (1<<CS00);
  TIMSK2 |= (1<<TOIE0);
  TCNT2 = 0;
  sei();
  lcd.begin(4);
  nunchuck_init();

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
      if (touchX > 108 && touchX < 234 && touchY > 98 && touchY < 117) {
        // If button is pressed go to game init
        initGame();
        inGame = true;
      }
    }

    if (inGame) {
      nunchuck_get_data();
      updatePlayers();
      _delay_ms(50);
    }

  }
  return 0;
}
