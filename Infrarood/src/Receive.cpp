/*
 * IRremote: IRrecvDemo - demonstrates receiving IR codes with IRrecv
 * An IR detector/demodulator must be connected to the input RECV_PIN.
 * Version 0.1 July, 2009
 * Copyright 2009 Ken Shirriff
 * http://arcfn.com
 */

#include <InfraRed.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdint.h>

#ifndef rec
#define rec 11
#endif

// void dump(decode_results *results) {
//   // Dumps out the decode_results structure.
//   // Call this after IRrecv::decode()
//   int count = results->rawlen;
//
//   Serial.print(results->value, HEX);
//   Serial.print(" (");
//   Serial.print(results->bits, DEC);
//   Serial.println(" bits)");
//   Serial.print("Raw (");
//   Serial.print(count, DEC);
//   Serial.print("): ");
//
//   for (int i = 1; i < count; i++) {
//     if (i & 1) {
//       Serial.print(results->rawbuf[i]*USECPERTICK, DEC);
//     }
//     else {
//       Serial.write('-');
//       Serial.print((unsigned long) results->rawbuf[i]*USECPERTICK, DEC);
//     }
//     Serial.print(" ");
//   }
//   Serial.println();
// }

int main(void) {
  Serial.begin(9600);
  RECEIVEIR RECEIVEIR(rec);

  decode_results results;


  RECEIVEIR.enableIR0(); // Start the receiver

  for(;;) {
    if (RECEIVEIR.checkData(&results)) {
      Serial.println("hoi");
      Serial.println(results.value, HEX);
      RECEIVEIR.goOn();

    }
    _delay_ms(1000);
  }
}
