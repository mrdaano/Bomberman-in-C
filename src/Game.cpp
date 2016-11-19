#include <avr/io.h>
#include <Wire.h>
#include <SPI.h>
#include <GraphicsLib.h>
#include <MI0283QT9.h>
#include <digitalWriteFast.h>
#include <stdbool.h>

MI0283QT9 lcd;
bool inGame = false;

void initMenu() {

}

void initGame() {

}


int main(void) {
  lcd.begin(4);
  lcd.fillScreen(RGB(255,255,255));
  for (;;) {
    /* code */
  }
  return 0;
}
