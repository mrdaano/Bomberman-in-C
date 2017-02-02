#ifndef H_HEADER
#define H_HEADER
#include <avr/io.h>
#include <Wire.h>
#include <time.h>
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

uint8_t readCalData(void);
void writeCalData(void);
void nunchuck_init();
void nunchuck_get_data();

extern NUNCHUCK_DATA nData;

extern MI0283QT9 lcd;
extern PLAYER player1;

extern String gameName;
extern bool inGame;
extern bool inSettings;
extern bool rendering;

extern String text;
extern String highscore;
extern uint16_t sec;
extern uint8_t teller;
#endif
