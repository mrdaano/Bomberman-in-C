#include "InfraRed.h"
#include "Timers.h"

int  RECEIVEIR::checkData(decode_results *results)
{
	results->rawbuf   = irparams.rawbuf;
	results->rawlen   = irparams.rawlen;

	results->overflow = irparams.overflow;

	if (irparams.rcvstate != STATE_STOP)  return false ;

	if (receivingIR(results)) {
    return true;
  }
	goOn();
	return false;
}

// Selecteren van de infrarood pin
RECEIVEIR::RECEIVEIR (int IRpin){
	irparams.IRpin = IRpin;
}

// Aanzetten van infrarood led
void RECEIVEIR::enableIR0(){
	cli(); // disable interrupts
	TIMER_CONFIG_NORMAL();
	TIMER_ENABLE_INTR;
	TIMER_RESET;
	sei();  // enable interrupts

	irparams.rawlen = 0;
	irparams.rcvstate = STATE_IDLE;

	// Set pin modes
	DDRB &= ~(1 << irparams.IRpin);
}

// legen voor de volgende infrarood stroom
void RECEIVEIR::goOn ( ){
	irparams.rcvstate = STATE_IDLE;
	irparams.rawlen = 0;
}
