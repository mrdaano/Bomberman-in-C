#include "header.h"

void updateScore(){
  //Draw lifes
  if(player1.lives == 3){
    lcd.fillCircle(260, 45, 4, RGB(204, 0, 0));
    lcd.fillCircle(275, 45, 4, RGB(204, 0, 0));
    lcd.fillCircle(290, 45, 4, RGB(204, 0, 0));
  } else if(player1.lives == 2){
    lcd.fillCircle(260, 45, 4, RGB(204, 0, 0));
    lcd.fillCircle(275, 45, 4, RGB(204, 0, 0));
    lcd.fillCircle(290, 45, 4, RGB(47, 79, 79));
  } else  if(player1.lives == 1){
    lcd.fillCircle(260, 45, 4, RGB(204, 0, 0));
    lcd.fillCircle(275, 45, 4, RGB(47, 79, 79));
    lcd.fillCircle(290, 45, 4, RGB(47, 79, 79));
  } else if(player1.lives == 0){
    lcd.fillCircle(260, 45, 4, RGB(47, 79, 79));
    lcd.fillCircle(275, 45, 4, RGB(47, 79, 79));
    lcd.fillCircle(290, 45, 4, RGB(47, 79, 79));

    // If player has zero lifes the game reset and go to home screen
    inGame = false;
    highscore = sec;
    sec = 0;
    text = "";
    _delay_ms(100);
    initMenu();
  }
}
