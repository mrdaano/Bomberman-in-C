#include <InfraRed.h>
#include <avr/interrupt.h>
#include <stdint.h>

#ifndef rec
#define rec 11
#endif

int main(void) {
  Serial.begin(9600);
  RECEIVEIR RECEIVEIR(rec);

  decode_results results;

  RECEIVEIR.enableIR0(); // Start the receiver

  for(;;) {
    if (RECEIVEIR.checkData(&results)) {
      Serial.println(results.value, DEC);

    }
  }
}
