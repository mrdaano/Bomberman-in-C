#include <avr/io.h>
#include <Wire.h>
#include <SPI.h>
#include <GraphicsLib.h>
#include <MI0283QT9.h>
#include <digitalWriteFast.h>
#include <stdbool.h>
#include <Arduino.h>
#include <util/delay.h>
#include <EEPROM.h>

#define maxBlocksInLength 4
#define maxBlocksInWidth 5

typedef struct {
  int x;
  int y;
  int lives;
  uint16_t color;
} PLAYER;

typedef struct {
  uint8_t x;
  uint8_t y;
} NUNCHUCK_DATA;

NUNCHUCK_DATA nData;

MI0283QT9 lcd;
PLAYER player1;

String gameName = "Bomberman";
bool inGame = false;

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
  player1.color = RGB(30,130,76);

  lcd.fillRect(player1.x, player1.y, 10, 10, player1.color);
}

void initMenu() {
  // Remove every content on screen
  lcd.fillScreen(RGB(0,0,0));
  lcd.drawText(20, 20, gameName, RGB(255,255,255), RGB(0,0,0), 4);

  String startText = "Start game";
  lcd.drawText(80, 100, startText, RGB(255,255,255), RGB(30,130,76), 2);

}

void initGame() {
  // Remove every content on screen
  lcd.fillScreen(RGB(255,255,255));
  // Create all the walls
  for (int w = 0; w < maxBlocksInWidth; w++) {
    for (int i = 0; i < maxBlocksInLength; i++) {
      lcd.fillRect(w * 57 + 30, i * 57 + 20, 30, 30, RGB(0,0,0));
    }
  }

  placePlayers();
}

void movePlayer(PLAYER *p, int UpDown, int LeftRight) {
  int u = p->x + 5;
  if (UpDown == 1) {
    u = p->x - 5;
  }

  int l = p->y + 2;

  if (LeftRight == 1) {
    l = p->y - 2;
  }
  lcd.fillRect(u, l, 10, 10, RGB(255,255,255));
  lcd.fillRect(p->x, p->y, 10, 10, p->color);
}

void updatePlayers() {
  bool moved = false;
  int UpDown = 6, LeftRight = 6;
  if (nData.x > 130 && player1.x+5 != 320) {
    player1.x = player1.x+5;
    moved = true;
    UpDown = 1;
  } else if(nData.x < 130 && player1.x-5 != 0) {
    player1.x = player1.x-5;
    moved = true;
    UpDown = 0;
  }

  if (nData.y > 126 && player1.y-2 > 0) {
    player1.y = player1.y-2;
    moved = true;
    LeftRight = 0;
  } else if (nData.y < 126 && player1.x-2 < 240) {
    player1.y = player1.y+2;
    moved = true;
    LeftRight = 1;
  }

  if (moved) {
    movePlayer(&player1, UpDown, LeftRight);
  }
}


int main(void) {
  init();
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
