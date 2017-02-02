#include "header.h"
#ifndef VARS
#define VARS

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

#endif
