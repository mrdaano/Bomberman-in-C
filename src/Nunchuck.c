#include <Wire.h>
#include <util/delay.h>

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

void nunchuck_send_zero() {
  Wire.beginTransmission(0x52);      // transmit to device 0x52
  Wire.write(0x00);           // sends one byte
  Wire.endTransmission();    // stop transmitting
}


// Not working yet
uint8_t * nunchuck_get_data() {
  uint8_t cnt;
  uint8_t status[6];
  Wire.requestFrom (0x52, 6); // request data from nunchuck
  while (Wire.available ()) {
      // receive byte as an integer
      status[cnt] =  Wire.read();
      cnt++;
  }

  nunchuck_send_zero();
  return status;
}
