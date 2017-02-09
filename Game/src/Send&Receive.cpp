// #include "InfraRed.h"
// #include <avr/interrupt.h>
// #include <stdint.h>
//
// SENDIR SENDIR;
// RECEIVEIR RECEIVEIR(rec);
// decode_results results;
//
// #if SEND
// int main(void){
// 	Serial.begin(9600);
// 	for (;;) {
// 		unsigned int data = 110;
// 		SENDIR.sendingIR(data, 8);
//     _delay_ms(2000);
// 	}
// }
// #endif
//
// #if RECEIVE
// int main(void) {
//   Serial.begin(9600);
//   RECEIVEIR.enableIR0(); // Start the receiver
//   for(;;) {
//     if (RECEIVEIR.checkData(&results)) {
//       Serial.println(results.value, DEC);
//     }
//   }
// }
// #endif
