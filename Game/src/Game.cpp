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
