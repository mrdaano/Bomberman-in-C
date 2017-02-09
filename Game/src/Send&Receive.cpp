// #include "InfraRed.h"
// #include <avr/interrupt.h>
// #include <stdint.h>
//
// SENDIR SENDIR;
// RECEIVEIR RECEIVEIR(rec);
// decode_results results;
//
// ISR(PCINT2_vect){
// 		RECEIVEIR.checkData(&results);
// 		Serial.println(results.value, DEC);
// }
// 
// #if SEND
// int main(void){
// 	Serial.begin(9600);
// 	for (;;) {
// 		unsigned int data = 110;
// 		SENDIR.sendingIR(data, 8);
// 		_delay_ms(2000);
// 	}
// }
// #endif
//
// #if RECEIVE
// int main(void) {
// 	PCICR |= (1 << PCIE2);
// 	PCMSK2 |= (1 << PCINT20);
//
//   Serial.begin(9600);
//   RECEIVEIR.enableIR0();
//
// 	while(1) {
// 		Serial.println("hoi");
// 		_delay_ms(500);
// 	}
// }
// #endif
