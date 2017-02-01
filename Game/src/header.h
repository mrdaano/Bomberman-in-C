#ifndef HEADER
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

#define HEADER

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
#endif
