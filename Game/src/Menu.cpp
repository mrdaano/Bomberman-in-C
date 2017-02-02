#include "header.h"

void initMenu() {
  // Remove every content on screen
  lcd.fillScreen(RGB(0,0,0));
  lcd.drawText(20, 20, gameName, RGB(255,255,255), RGB(0,0,0), 4);

  String startText = "Start game";
  lcd.drawText(40, 100, startText, RGB(255,255,255), RGB(30,130,76), 3);

  String settingsTxt = "Settings";
  lcd.drawText(40, 150, settingsTxt, RGB(255,255,255), RGB(34,167,240), 3);

  String highScoreText = "Highscore:";
  lcd.drawText(40, 200, highScoreText, RGB(255,255,255), RGB(0,0,0), 1);
  lcd.drawText(120, 200, highscore, RGB(255,255,255), RGB(0,0,0), 1);

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

//Change brightness
int brightness = 5;
void turnBrightnessUp(int brightness){
    lcd.led(brightness * 10);
}

void turnBrightnessDown(int brightness){
    lcd.led(brightness * 10);
}
