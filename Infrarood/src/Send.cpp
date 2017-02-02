#include <InfraRed.h>
#include <util/delay.h>

SENDIR SENDIR;


int main(void){
	Serial.begin(9600);
	for (;;) {
		unsigned int data = 28;

		SENDIR.sendingIR(data, 8);
   _delay_ms(5000);
	}
}
